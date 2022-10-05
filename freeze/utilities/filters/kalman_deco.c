//
//  kalman2d.c
//  ots-proto
//
//  Created by Matthew Fonken on 4/10/22.
//

#include "kalman2d.h"

/* Based on: https://tinyurl.com/sd3f2s8m */

void Kalman2D_Init( kalman2d_t * k, floatp u[2], floatp process_noise, floatp x_std_meas, floatp y_std_meas )
{
    Kalman2D.Reset( k );
    
    k->x.ax = (u == NULL) ? 0. : u[0];
    k->x.ay = (u == NULL) ? 0. : u[1];
    
    k->R[0][0] = x_std_meas;
    k->R[0][1] = 0;
    k->R[1][0] = 0;
    k->R[1][1] = y_std_meas;
    
    k->process_noise_sq = process_noise * process_noise;
    
    k->t =
#ifdef SET_DT_SEC
        SET_DT_SEC;
#else
    (floatp)TIMESTAMP(TIME_SEC);
#endif
}

void Kalman2D_Reset( kalman2d_t * k )
{
    memset( &k->x, 0, sizeof(k->x) );
//    for(int i = 0; i < 4; i++) ((floatp*)&k->state.px)[i] = i + 1;
//    k->state.ax = 1;
//    k->state.ay = 1;
//    memset( &k->meas_var, 0, 2 * sizeof(floatp) );
    memset( &k->B, 0, 4 * 2 * sizeof(floatp) );
    memset( &k->Q, 0, 4 * 4 * sizeof(floatp) );
    
    Matrix.Eye( (floatp*)k->A, 4, 4 );
    Matrix.Eye( (floatp*)k->P, 4, 4 );
    Matrix.Eye( (floatp*)k->H, 2, 4 );
}

physical_2d_t * Kalman2D_Test( kalman2d_t * k, floatp x_new[4], bool update_A )
{
    if(update_A)
    {
        floatp dt =
#ifdef SET_DT_SEC
        SET_DT_SEC;
#else
        ((floatp)TIMESTAMP(TIME_SEC)) - k->t;
#endif
        k->A[0][2] = dt;
        k->A[1][3] = dt;
    }
    
    // Predict next state:
    // x = A.x_ + B.u_ where u = [[ax], [ay]]
    floatp r1[4], r2[4], *x = (floatp*)&k->x.px, *Ap = (floatp*)k->A;
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "A.x:\n");
    Matrix.Dot( Ap, x, false, r1, 4, 1, 4 );
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "B.a:\n");
    Matrix.Dot( (floatp*)k->B, (floatp*)&k->x.ax, false, r2, 4, 1, 2 );
    Matrix.AddSub( r1, r2, x_new, 1, 4, true );
    
    return &k->x;
}

void Kalman2D_Predict( kalman2d_t * k )
{
    // Time update
    floatp t_ = (floatp)TIMESTAMP(TIME_SEC);
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
    Kalman2D.Test( k, (floatp*)&k->x.px, true );
    
    // Update error covariance:
    // P = A.P.AT + Q
    floatp r3[4][4], r4[4][4];
    floatp *r3p = (floatp*)r3, *r4p = (floatp*)r4, *Ap = (floatp*)k->A;;
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "A.P:\n");
    Matrix.Dot( Ap, (floatp*)k->P, false, r3p, 4, 4, 4);
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "(A.P).AT:\n");
    Matrix.Dot( r3p, Ap, true, r4p, 4, 4, 4);
    Matrix.AddSub( r4p, (floatp*)k->Q, (floatp*)k->P, 4, 4, true );
}

void Kalman2D_Update( kalman2d_t * k, floatp z[2] )
{
    floatp PHT[4][2], *Hp = (floatp*)k->H;
    floatp *PHTp = (floatp*)PHT;
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "P.HT\n");
    Matrix.Dot( (floatp*)k->P, Hp, true, PHTp, 4, 2, 4 );
    
    floatp S[2][2];
    floatp *Sp = (floatp*)S;
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "H.(P.HT)\n");
    Matrix.Dot( Hp, PHTp, false, Sp, 2, 2, 4 );
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "H.(P.HT) + R\n");
    Matrix.AddSub( Sp, (floatp*)k->R, Sp, 2, 2, true );
    
    floatp Sinv[2][2];
    floatp *Kp  = (floatp*)k->K;
    Matrix.inv22( S, Sinv );
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "(P.HT).Sinv\n");
    Matrix.Dot( PHTp, (floatp*)Sinv, false, Kp, 4, 2, 2 );
    
    floatp r3[2], r4[4], *x = (floatp*)&k->x.px;
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "H.x\n");
    Matrix.Dot( (floatp*)k->H, x, false, r3, 2, 1, 4 );
    Matrix.AddSub( z, r3, r3, 2, 2, false );
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "K.z...\n");
    Matrix.Dot( Kp, r3, false, r4, 4, 1, 2 );
    Matrix.AddSub( x, r4, x, 1, 4, true );
    
    floatp r5[4][4], r6[4][4];
    floatp *r5p = (floatp*)r5;
    floatp *r6p = (floatp*)r6;
    Matrix.Eye( (floatp*)r5, 4, 4 );
    
    floatp K44[4][4], H44[4][4];
    Matrix.ZPad( (floatp*)k->K, 4, 2, (floatp*)K44, 4, 4);
    Matrix.ZPad( (floatp*)k->H, 2, 4, (floatp*)H44, 4, 4);
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "K.H\n");
    Matrix.Dot( (floatp*)K44, (floatp*)H44, false, r6p, 4, 4, 4);
    Matrix.AddSub( r5p, r6p, r5p, 4, 4, false );
    
    floatp r7[4][4];
    floatp *r7p = (floatp*)r7;
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "(I - K.H).P\n");
    Matrix.Dot( r5p, (floatp*)k->P, false, r7p, 4, 4, 4);
    memcpy( k->P, r7p, 4 * 4 * sizeof(floatp));
}

physical_2d_t * Kalman2D_Step( kalman2d_t * k, floatp z[2] )
{
    Kalman2D.Predict( k );
    Kalman2D.Update( k, z );
    return &k->x;
}

void Kalman2D_Print( kalman2d_t * k )
{
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "p: (%.4f, %.4f) | v: (%.4f, %.4f)  | a:(%.4f, %.4f) \n", k->x.px, k->x.py, k->x.vx, k->x.vy, k->x.ax, k->x.ay);
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "P:\t[%.4f][%.4f][%.4f][%.4f]\n", k->P[0][0], k->P[0][1], k->P[0][2], k->P[0][3]);
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "  \t[%.4f][%.4f][%.4f][%.4f]\n", k->P[1][0], k->P[1][1], k->P[1][2], k->P[1][3]);
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "  \t[%.4f][%.4f][%.4f][%.4f]\n", k->P[2][0], k->P[2][1], k->P[2][2], k->P[2][3]);
    LOG_KALMAN2D(DEBUG_KALMAN2D_PRIO, "  \t[%.4f][%.4f][%.4f][%.4f]\n", k->P[3][0], k->P[3][1], k->P[3][2], k->P[3][3]);
}

const struct kalman2d_functions Kalman2D =
{
    .Init = Kalman2D_Init,
    .Reset = Kalman2D_Reset,
    .Test = Kalman2D_Test,
    .Predict = Kalman2D_Predict,
    .Update = Kalman2D_Update,
    .Step = Kalman2D_Step,
    .Print = Kalman2D_Print
};
