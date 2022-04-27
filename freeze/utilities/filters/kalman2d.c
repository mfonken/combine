//
//  kalman2d.c
//  ots-proto
//
//  Created by Matthew Fonken on 4/10/22.
//

#include "kalman2d.h"

void Kalman2D_Init( kalman2d_t * k, floatp process_noise, floatp x_std_meas, floatp y_std_meas )
{
    memset(&k->state, 0, sizeof(k->state));
//    for(int i = 0; i < 4; i++) ((floatp*)&k->state.px)[i] = i + 1;
//    k->state.ax = 1;
//    k->state.ay = 1;
    memset(&k->meas_var, 0, 2 * sizeof(floatp));
    memset(&k->B, 0, 4 * 2 * sizeof(floatp));
    memset(&k->Q, 0, 4 * 4 * sizeof(floatp));
    
    k->R[0][0] = x_std_meas;
    k->R[0][1] = 0;
    k->R[1][0] = 0;
    k->R[1][1] = y_std_meas;
    
    Matrix.eye( (floatp*)k->A, 4, 4 );
    Matrix.eye( (floatp*)k->P, 4, 4 );
    Matrix.eye( (floatp*)k->H, 2, 4 );
    
    k->process_noise_sq = process_noise * process_noise;
    
    k->t =
#ifdef SET_DT_SEC
        SET_DT_SEC;
#else
    (floatp)TIMESTAMP() * 1e-3;
#endif
}

void Kalman2D_Test( kalman2d_t * k, floatp x_new[4], bool update_A )
{
    if(update_A)
    {
        floatp dt =
#ifdef SET_DT_SEC
        SET_DT_SEC;
#else
        ((floatp)TIMESTAMP() * 1e-3) - k->t;
#endif
        k->A[0][2] = dt;
        k->A[1][3] = dt;
    }
    
    // Predict next state:
    // x = A.x_ + B.u_ where u = [[ax], [ay]]
    floatp r1[4], r2[4], *x = (floatp*)&k->state.px, *Ap = (floatp*)k->A;
    LOG_K2(DEBUG_2, "A.x:\n");
    Matrix.dot( Ap, x, false, r1, 4, 1, 4 );
    LOG_K2(DEBUG_2, "B.a:\n");
    Matrix.dot( (floatp*)k->B, (floatp*)&k->state.ax, false, r2, 4, 1, 2 );
    Matrix.addsub( r1, r2, x_new, 1, 4, true );
}

void Kalman2D_Predict( kalman2d_t * k )// , int l )
{
//    int8_t ll = l * l;
    // Time update
    floatp t_ = (floatp)TIMESTAMP() * 1e-3;
    floatp dt =
#ifdef SET_DT_SEC
        SET_DT_SEC;
#else
        t_ - k->t;
#endif
//    printf("∆t: %2fms\n", dt);
    k->t = t_;
    floatp dt_sq = dt * dt;
    floatp dt_hfsq = dt_sq / 2;
    
    // Update matrices with dt
    k->A[0][2] = dt;
    k->A[1][3] = dt;
    
    k->B[0][0] = dt_hfsq;
    k->B[1][1] = dt_hfsq;
    k->B[2][0] = dt;
    k->B[3][1] = dt;
    
    k->Q[0][0] = dt_hfsq * dt_hfsq * k->process_noise_sq; // dt^4/4 * n^2
    k->Q[0][2] = dt_hfsq * dt * k->process_noise_sq;      // dt^3/2 * n^2
    k->Q[1][1] = k->Q[0][0];
    k->Q[1][3] = k->Q[0][2];
    k->Q[2][0] = k->Q[0][2];
    k->Q[2][2] = dt_sq * k->process_noise_sq;             // dt^2 * n^2
    k->Q[3][1] = k->Q[0][2];
    k->Q[3][3] = dt_sq * k->process_noise_sq;
    
    // Predict next state:
    // x = A.x_ + B.u_ where u = [[ax], [ay]]
    Kalman2D.test( k, (floatp*)&k->state.px, false );
    
    // Update error covariance:
    // P = A.P.AT + Q
    floatp r3[4][4], r4[4][4];
    floatp *r3p = (floatp*)r3, *r4p = (floatp*)r4, *Ap = (floatp*)k->A;;
    LOG_K2(DEBUG_2, "A.P:\n");
    Matrix.dot( Ap, (floatp*)k->P, false, r3p, 4, 4, 4);
    LOG_K2(DEBUG_2, "(A.P).AT:\n");
    Matrix.dot( r3p, Ap, true, r4p, 4, 4, 4);
    Matrix.addsub( r4p, (floatp*)k->Q, (floatp*)k->P, 4, 4, true );
}

void Kalman2D_Update( kalman2d_t * k, floatp z[2] )
{
    floatp PHT[4][2], *Hp = (floatp*)k->H;
    floatp *PHTp = (floatp*)PHT;
    LOG_K2(DEBUG_2, "P.HT\n");
    Matrix.dot( (floatp*)k->P, Hp, true, PHTp, 4, 2, 4 );
    
    floatp S[2][2];
    floatp *Sp = (floatp*)S;
    LOG_K2(DEBUG_2, "H.(P.HT)\n");
    Matrix.dot( Hp, PHTp, false, Sp, 2, 2, 4 );
    LOG_K2(DEBUG_2, "H.(P.HT) + R\n");
    Matrix.addsub( Sp, (floatp*)k->R, Sp, 2, 2, true );
    
    floatp K[4][2], Sinv[2][2];
    floatp *Kp  = (floatp*)K;
    Matrix.inv22( S, Sinv );
    LOG_K2(DEBUG_2, "(P.HT).Sinv\n");
    Matrix.dot( PHTp, (floatp*)Sinv, false, Kp, 4, 2, 2 );
    
    floatp r3[2], r4[4], *x = (floatp*)&k->state.px;
    LOG_K2(DEBUG_2, "H.x\n");
    Matrix.dot( (floatp*)k->H, x, false, r3, 2, 1, 4 );
    Matrix.addsub( z, r3, r3, 2, 2, false );
    LOG_K2(DEBUG_2, "K.z...\n");
    Matrix.dot( Kp, r3, false, r4, 4, 1, 2 );
    Matrix.addsub( x, r4, x, 1, 4, true );
    
    floatp r5[4][4], r6[4][4];
    floatp *r5p = (floatp*)r5;
    floatp *r6p = (floatp*)r6;
    Matrix.eye( (floatp*)r5, 4, 4 );
    
    floatp K44[4][4], H44[4][4];
    Matrix.zpad( (floatp*)K, 4, 2, (floatp*)K44, 4, 4);
    Matrix.zpad( (floatp*)k->H, 2, 4, (floatp*)H44, 4, 4);
    LOG_K2(DEBUG_2, "K.H\n");
    Matrix.dot( (floatp*)K44, (floatp*)H44, false, r6p, 4, 4, 4);
    Matrix.addsub( r5p, r6p, r5p, 4, 4, false );
    
    floatp r7[4][4];
    floatp *r7p = (floatp*)r7;
    LOG_K2(DEBUG_2, "(I - K.H).P\n");
    Matrix.dot( r5p, (floatp*)k->P, false, r7p, 4, 4, 4);
    memcpy( k->P, r7p, 4 * 4 * sizeof(floatp));
}

const struct kalman2d_functions Kalman2D =
{
    .init = Kalman2D_Init,
    .test = Kalman2D_Test,
    .predict = Kalman2D_Predict,
    .update = Kalman2D_Update
};
