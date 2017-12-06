//
//  imu_stubs.cpp
//  combine_core
//
//  Created by Matthew Fonken on 11/17/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "imu_stubs.h"
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MIN_PACKET_LEN 54

#define BUFFER_LENGTH   (1 << 7)

#define PACKET_DEL '\r'

double mag_cal[] = { -2.396, 38.040, 1.093 };

const char port[] = "/dev/tty.usbmodem14121";
const char port_alt[] = "/dev/tty.usbmodem14221";
int uart0_filestream = -1;
char line[BUFFER_LENGTH];

void Init_SERCOM()
{
    printf("Initializing SERCOM at %s\n", port);
    uart0_filestream = open(port, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        printf("Trying alternate port at %s\n", port_alt);
        uart0_filestream = open(port_alt, O_RDWR | O_NOCTTY | O_NDELAY);
        if (uart0_filestream == -1)
            printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
        else
            printf("Successfull opened alternate port.\n");
    }
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

void Read_SERCOM_IMU_Packet( LSM9DS1_t * imu )
{
    char buffer[BUFFER_LENGTH];
    write(uart0_filestream, buffer, BUFFER_LENGTH-1);
    int bytes_read = -1, ptr = 0, isnl = 0;
    while( !isnl && ptr < BUFFER_LENGTH-1)
    {
        bytes_read = 0;
        while(bytes_read <= 0) bytes_read = (int)read(uart0_filestream, buffer, (size_t)BUFFER_LENGTH);
        for(int i = ptr, j = 0; i < ptr + bytes_read; i++, j++)
        {
            if(buffer[j] == PACKET_DEL)
            {
                line[i] = PACKET_DEL;
                isnl = 1;
            }
            else if(buffer[j] != '\n' && buffer[j] != '\\') line[i] = buffer[j];
            buffer[j] = 0;
        }
        ptr += bytes_read;
    }
    line[ptr] = ',';
    line[++ptr] = '\0';
    if(ptr < MIN_PACKET_LEN) return;
    
#ifdef PACKET_DEBUG
    printf("\nR(%d): %s\n", ptr, line);
#endif
    
    double v[9];
    tokenifyPacket( line, ptr, v);
    
    if( v[0] == 0xffff ) return;
#ifdef PACKET_DEBUG
    for(int i = 0; i < 9; i++)
        printf("(%d)%.2f ", i, v[i]);
    printf("\n");
#endif
    
    imu->data.accel[0] = v[0];
    imu->data.accel[1] = v[1];
    imu->data.accel[2] = v[2];
    imu->data.gyro[0]  = v[3];
    imu->data.gyro[1]  = v[4];
    imu->data.gyro[2]  = v[5];
    imu->data.mag[1]   = v[6] - mag_cal[0];
    imu->data.mag[0]   = v[7] - mag_cal[1];
    imu->data.mag[2]   =-(v[8] - mag_cal[2]);
}

void IMU_Init(LSM9DS1_t * a)
{
    Init_SERCOM();
    return;
}
void IMU_Update_All(LSM9DS1_t * a)
{
    Read_SERCOM_IMU_Packet(a);
    IMU_Normalize_All(a);
    IMU_Update_Roll(a);
    IMU_Update_Pitch(a);
    IMU_Update_Yaw(a);
    return;
}

void IMU_Normalize_All( LSM9DS1_t * imu )
{
    IMU_Normalize_Accel(imu);
    IMU_Normalize_Mag(imu);
}
void IMU_Normalize_Accel( LSM9DS1_t * imu )
{
    double x,y,z,n;
    x = imu->data.accel[0];
    y = imu->data.accel[1];
    z = imu->data.accel[2];
    
    n = sqrt( x*x + y*y + z*z );
    imu->data.accel[0] /= n;
    imu->data.accel[1] /= n;
    imu->data.accel[2] /= n;
}
void IMU_Normalize_Mag( LSM9DS1_t * imu )
{
    double x,y,z,n;
    x = imu->data.mag[0];
    y = imu->data.mag[1];
    z = imu->data.mag[2];
    
    n = sqrt( x*x + y*y + z*z );
    imu->data.mag[0] /= n;
    imu->data.mag[1] /= n;
    imu->data.mag[2] /= n;
}

void IMU_Update_Roll( LSM9DS1_t * imu )
{
    /* AN4248: Eq. 13 */
    imu->data.roll = atan2( imu->data.accel[1], imu->data.accel[2] );
    
    /* AN3461: Eq. 37 */
    //    double den = sqrt( ( ( imu->data.accel[1] * imu->data.accel[1] ) + ( imu->data.accel[2] * imu->data.accel[2] ) ) );
    //    imu->data.roll = atan2( -imu->data.accel[0], den );
}

/* Note: Requires updated roll */
void IMU_Update_Pitch( LSM9DS1_t * imu )
{
    /* AN4248: Eq. 14 */
    double den = ( imu->data.accel[1] * sin( imu->data.roll ) ) + ( imu->data.accel[2] * cos ( imu->data.roll ) );
    imu->data.pitch = atan2( -imu->data.accel[0], den );
    
    /* AN3461: Eq. 38 */
    //    double den = sign( imu->data.accel[2] ) * sqrt( ( ( imu->data.accel[2] * imu->data.accel[2] ) + ( MU * ( imu->data.accel[0] * imu->data.accel[0] ) ) ) );
    //    imu->data.pitch = atan2( imu->data.accel[1], den );
}

/* Note: Requires updated pitch and roll */
void IMU_Update_Yaw( LSM9DS1_t * imu )
{
    /* NOTE: Magnetometer X and Y axes are switched between AN4248<>LSM9DS1 */
    
    /* AN4248: Eq. 22 */
    double sin_phi   = sin( imu->data.roll );
    double sin_theta = sin( imu->data.pitch );
    double cos_phi   = cos( imu->data.roll );
    double cos_theta = cos( imu->data.pitch );
    double num = ( imu->data.mag[2] * sin_phi ) - ( imu->data.mag[1] * cos_phi );
    double den = ( imu->data.mag[0] * cos_theta ) + ( imu->data.mag[1] * ( sin_theta * sin_phi ) ) + ( imu->data.mag[2] * ( sin_theta * cos_phi ) );
    imu->data.yaw = -atan2( -num, den );
}

//double IMU_Roll_Error_Get( LSM9DS1_t * imu )
//{
//    double sin_phi   = sin( imu->data.roll );
//    double sin_theta = sin( imu->data.pitch );
//    double cos_phi   = cos( imu->data.roll );
//    double cos_theta = cos( imu->data.pitch );
//    double cos_theta_cos_phi = cos_theta * cos_phi;
//    double mu_sin_2_theta = MU * ( sin_theta * sin_theta );
//    double factor = sqrt( ( cos_theta_cos_phi * cos_theta_cos_phi ) + mu_sin_2_theta );
//    double num = sin_phi * ( cos_theta_cos_phi - factor );
//    double den = ( cos_theta * ( sin_phi * sin_phi ) ) + ( cos_phi * factor );
//    return atan2( num, den );
//}

void IMU_Get_Non_Grav( LSM9DS1_t * imu, vec3_t * ngacc )
{
    IMU_Update_All( imu );
    
    quaternion_t q;
    
    /* Create a vector of accelerometer values */
    ang3_t a;
    a.x = imu->data.pitch;
    a.y = imu->data.roll;
    a.z = 0;
    Euler_To_Quaternion(&a, &q);
    
    vec3_t g,r;
    g.i =  0;
    g.j =  0;
    g.k = -1;
    Rotate_Vector_By_Quaternion(&g, &q, &r);

    ngacc->i = imu->data.accel[0] - r.i;
    ngacc->j = imu->data.accel[1] - r.j;
    ngacc->k = imu->data.accel[2] - r.k;
}


void tokenifyPacket( char * a, int l, double * d)
{
    char **tokens;
    tokens = str_split(a, ',');
    
    if (tokens)
    {
        int i;
        if(*(tokens)[0] != 'r')
        {
            d[0] = 0xffff;
            return;
        }
        for (i = 1; *(tokens + i) && i <= 9; i++)
        {
            char * c = *(tokens + i);
#ifdef PACKET_DEBUG
            size_t l = sizeof(c);
            printf("%d - token %s(%lu)\n", i, c, l);
#endif
            if(l) d[i-1] = atof(c);
            else
            {
                d[0] = 0xffff;
#ifdef PACKET_DEBUG
                printf("Ending tokenize\n");
#endif
                return;
            }
            free(*(tokens + i));
        }
        if(i <= 9)
        {
            d[0] = 0xffff;
#ifdef PACKET_DEBUG
            printf("Tokenize failed\n");
#endif
        }
        free(tokens);
    }
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    
    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    
    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);
    
    /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
    count++;
    
    result = malloc(sizeof(char*) * count);
    
    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);
        
        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
//        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    
    return result;
}
