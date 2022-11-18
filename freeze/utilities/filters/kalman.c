#include "kalman.h"
//#ifndef TIMESTAMP
#include "timestamp.h"

#define DEBUG_KALMAN_PRIO DEBUG_2

//#endif
/* Based on: https://tinyurl.com/39mx3pu3 */

void Kalman_Init( kalman_t * k, floatp p, floatp process_noise, floatp std_meas, floatp lifespan, floatp p_min, floatp p_max )
{
    Kalman.Reset( k, p );
    
    k->R = std_meas * std_meas;
    k->process_noise_sq = process_noise * process_noise;

//    k->lifespan = lifespan;
//    k->p_min = p_min;
//    k->p_max = p_max;
//    k->valid = false;
}

void Kalman_Reset( kalman_t * k, floatp p )
{
    memset( &k->x, 0, sizeof(k->x) );
    memset( &k->B, 0, 2 * sizeof(floatp) );
    memset( &k->Q, 0, 2 * 2 * sizeof(floatp) );
    
    Matrix.Eye( (floatp*)k->A, 2, 2 );
    Matrix.Eye( (floatp*)k->P, 2, 2 );
    Matrix.Eye( (floatp*)k->H, 1, 2 );

    k->x.p = p;
    k->t = (floatp)TIMESTAMP(TIME_SEC);
    k->t_origin = k->t;
}

void Kalman_Test( kalman_t * k, floatp x_new[2], bool update )
{
    floatp A01_ = k->A[0][1];
    floatp dt =
#ifdef SET_DT_SEC
    SET_DT_SEC;
#else
    ((floatp)TIMESTAMP(TIME_SEC)) - k->t;
#endif
//    printf("∆t: %.2fms (%.2f | %.2f)\n ", dt * 1000.0, (floatp)TIMESTAMP(TIME_SEC), k->t);
    k->A[0][1] = dt;
    
//    printf("A:%.2f(%.2f)\n", k->x.p, k->x.v);
    // Predict next state:
    // x = A.x_ + B.u_ where u = [a]
    floatp r1[2], r2[2], *x = (floatp*)&k->x.p, *Ap = (floatp*)k->A;
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "A.x:\n");
    Matrix.Dot( Ap, x, false, r1, 2, 1, 2 );
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "B.a:\n");
    Matrix.Dot( (floatp*)k->B, (floatp*)&k->x.a, false, r2, 2, 1, 1 );
    Matrix.AddSub( r1, r2, ( update ? (floatp*)&k->x.p : x_new ), 1, 2, true );
//    printf("B:%.2f(%.2f)\n", k->x.p, k->x.v);
    
    if(k->x.p > 1000)
        printf("!");
    if(k->x.v > 200)
        printf("!");
    
    if(!update)
        k->A[0][1] = A01_;
}

floatp Kalman_TestSelf( kalman_t * k )
{
    floatp x_curr[2] = { k->x.p, k->x.v };
    Kalman.Test( k, x_curr, false );
    return x_curr[0];
}

void Kalman_Predict( kalman_t * k )
{
    floatp t_ = (floatp)TIMESTAMP(TIME_SEC);
    floatp dt =
#ifdef SET_DT_SEC
        SET_DT_SEC;
#else
        t_ - k->t;
#endif
    floatp dt_sq = dt * dt;
    floatp dt_hfsq = dt_sq / 2;
    
    // Update matrices with dt
    k->A[0][1] = dt;
    
    k->B[0] = dt_hfsq;
    k->B[1] = dt;
    
    k->Q[0][0] = dt_hfsq * dt_hfsq * k->process_noise_sq;
    k->Q[0][1] = dt_hfsq * dt * k->process_noise_sq;
    k->Q[1][0] = k->Q[0][1];
    k->Q[1][1] = dt_hfsq;
    
    // Predict next state:
    // x = A.x_ + B.u_ where u = [a]
    Kalman.Test( k, (floatp*)&k->x.p, true );
    
    // Update error covariance:
    // P = A.P.AT + Q
    floatp r3[2][2], r4[2][2];
    floatp *r3p = (floatp*)r3, *r4p = (floatp*)r4, *Ap = (floatp*)k->A;
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "A.P:\n");
    Matrix.Dot( Ap, (floatp*)k->P, false, r3p, 2, 2, 2);
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "(A.P).AT:\n");
    Matrix.Dot( r3p, Ap, true, r4p, 2, 2, 2);
    Matrix.AddSub( r4p, (floatp*)k->Q, (floatp*)k->P, 2, 2, true );
    
    k->t = t_;
}

void Kalman_Update( kalman_t * k, floatp z )
{
    floatp PHT[2], *Hp = (floatp*)k->H;
    floatp *PHTp = (floatp*)PHT;
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "P.HT\n");
    Matrix.Dot( (floatp*)k->P, Hp, true, PHTp, 2, 1, 2 );
    
    // S = H.P.HT + R
    floatp S;
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "S = H.(P.HT)\n");
    Matrix.Dot( Hp, PHTp, false, &S, 1, 1, 2 );
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "S += R\n");
    S += k->R;
    
    // K = P.HT.S^-1
    floatp Sinv = 1 / S;
    floatp *Kp  = (floatp*)k->K;
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "(P.HT).Sinv\n");
    Matrix.Dot( PHTp, (floatp*)&Sinv, false, Kp, 2, 1, 1 );
    
    floatp r3, r4[2], *x = (floatp*)&k->x.p;
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "H.x\n");
    Matrix.Dot( (floatp*)k->H, x, false, &r3, 1, 1, 2 );
    r3 = z - r3;
//    Matrix.AddSub( z, r3, r3, 2, 2, false );
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "K.z...\n");
    Matrix.Dot( Kp, &r3, false, r4, 2, 1, 1 );
    Matrix.AddSub( x, r4, x, 1, 2, true );
    
    floatp r5[2][2], r6[2][2];
    floatp *r5p = (floatp*)r5;
    floatp *r6p = (floatp*)r6;
    Matrix.Eye( (floatp*)r5, 2, 2 );
    
    floatp K22[2][2], H22[2][2];
    Matrix.ZPad( (floatp*)k->K, 2, 1, (floatp*)K22, 2, 2);
    Matrix.ZPad( (floatp*)k->H, 1, 2, (floatp*)H22, 2, 2);
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "K.H\n");
    Matrix.Dot( (floatp*)K22, (floatp*)H22, false, r6p, 2, 2, 2);
    Matrix.AddSub( r5p, r6p, r5p, 2, 2, false );
    
    floatp r7[2][2];
    floatp *r7p = (floatp*)r7;
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "(I - K.H).P\n");
    Matrix.Dot( r5p, (floatp*)k->P, false, r7p, 2, 2, 2);
    memcpy( k->P, r7p, 2 * 2 * sizeof(floatp));
};

physical_t * Kalman_Step( kalman_t * k, floatp z )
{
    Kalman_Predict( k );
    Kalman_Update( k, z );
    return &k->x;
}

floatp Kalman_Confidence( kalman_t * k );
inline floatp Kalman_Confidence( kalman_t * k )
{
    return k->K[0];
}

void Kalman_Print( kalman_t * k )
{
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "p: %.4f | v: %.4f | a:%.4f\n", k->x.p, k->x.v, k->x.a);
//    LOG_KALMAN(DEBUG_KALMAN_PRIO, "Bias: %.4f | Var: %.4f | Scr:%.4f\n", k->bias, k->variance, k->score);
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "K:\t[%.4f][%.4f]\n", k->K[0], k->K[1]);
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "P:\t[%.4f][%.4f]\n", k->P[0][0], k->P[0][1]);
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "  \t[%.4f][%.4f]\n", k->P[1][0], k->P[1][1]);
}

//#ifdef MATVEC_LIB
//gaussian1d_t GetGaussian1DKalman( kalman_filter_t * k )
//{
//    return (gaussian1d_t){ k->value, k->P[0][0] };
//}
//#endif

const struct kalman_functions Kalman =
{
    .Init = Kalman_Init,
    .Reset = Kalman_Reset,
    .Test = Kalman_Test,
    .TestSelf = Kalman_TestSelf,
    .Predict = Kalman_Predict,
    .Update = Kalman_Update,
    .Step = Kalman_Step,
    .Confidence = Kalman_Confidence,
    .Print = Kalman_Print,
#ifdef MATVEC_LIB
    .Gaussian1D = GetGaussian1DKalman
#endif
};
