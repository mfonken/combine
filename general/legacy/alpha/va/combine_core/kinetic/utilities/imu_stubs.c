//
//  imu_stubs.cpp
//  combine_core
//
//  Created by Matthew Fonken on 11/17/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "imu_stubs.h"

void tokenifyPacket( char * a, int l, int n, char s, double * d)
{
    char **tokens;
    tokens = str_split(a, ',');
    
    if (tokens)
    {
        int i;
        if(*(tokens)[0] != s)
        {
            d[0] = 0xffff;
            return;
        }
        for (i = 1; *(tokens + i) && i <= n; i++)
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
        if(i <= n)
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
