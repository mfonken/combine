//
//  matrix.c
//  ots-proto
//
//  Created by Matthew Fonken on 4/12/22.
//

#include "matrix.h"

void Matrix_Eye( floatp *m, uint8_t rows, uint8_t cols )
{
    uint8_t x, y, yx = 0;
    for( y = 0 ; y < rows; y++)
        for( x = 0; x < cols; x++, yx++ )
            m[yx] = x == y ? 1 : 0;
}

void Matrix_Dot( floatp a[], floatp b[], bool T_b, floatp r[], uint8_t rows, uint8_t cols, uint8_t inner )
{
    int8_t x, y = 0, i;
    uint8_t yi = 0, xi = 0, yx = 0;
    floatp s;
    uint8_t T_b_1 = T_b ? inner : 1;
    uint8_t T_b_2 = T_b ? 1 : cols;
    
    for( y = 0 ; y < rows; y++ )
    {
        for( x = 0; x < cols; x++, yx++ )
        {
            xi = x * T_b_1;
            yi = y * inner;
            s = 0;
            for( i = 0; i < inner; i++, yi++, xi += T_b_2 )
            {
                floatp r = a[yi] * b[xi];
                s += r;
                LOG_MTX(DEBUG_2, "%.1f[%d] x %.1f[%d] = %.1f [%.1f]\n", a[yi], yi, b[xi], xi, r, s );
            }
            r[yx] = s;
            LOG_MTX(DEBUG_2, "\n");
        }
    }
}

void Matrix_AddSub( floatp a[], floatp b[], floatp r[], uint8_t rows, uint8_t cols, bool add )
{
    int8_t x, y = rows - 1;
    floatp *ap = a, *bp = b, *rp = r;
    for( ; y >= 0; y-- )
        for( x = cols - 1; x >= 0; x-- )
            *(rp++) = *(ap++) + *(bp++) * ( add ? 1 : -1 );
}

void Matrix_ZPad( floatp m[], uint8_t rows_in, uint8_t cols_in, floatp r[], uint8_t rows_out, uint8_t cols_out)
{
    if( ( cols_in > cols_out ) || ( rows_in > rows_out ) ) return;
    int8_t x, y = 0;
    floatp *mp = m, *rp = r;
    for( ; y < rows_in; y++ )
    {
        for( x = 0; x < cols_in; x++ )
            *(rp++) = *(mp++);
        for( ; x < cols_out; x++ )
            *(rp++) = 0;
    }
    memset( rp, 0, cols_out * ( rows_out - y ) );
}

void Matrix_Inv22( floatp m[2][2], floatp r[2][2])
{
    floatp det = ( m[0][0] * m[1][1] ) - ( m[1][0] * m[0][1] );
    if( det == 0 )
    {
        memset( r, 0, 4 * sizeof(floatp));
        return;
    }
    floatp det_ = 1 / det;
    r[0][0] = m[1][1] * det_;
    r[0][1] = -m[0][1] * det_;
    r[1][0] = -m[1][0] * det_;
    r[1][1] = m[0][0] * det_;
}

const matrix_functions Matrix =
{
    .eye = Matrix_Eye,
    .dot = Matrix_Dot,
    .addsub = Matrix_AddSub,
    .zpad = Matrix_ZPad,
    .inv22 = Matrix_Inv22
};
