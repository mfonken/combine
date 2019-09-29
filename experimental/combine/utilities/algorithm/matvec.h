//
//  matvec.h
//  combine
//
//  Created by Matthew Fonken on 9/3/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifndef matvec_h
#define matvec_h

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#ifndef ZDIV
#define ZDIV_LNUM 1 << 10
#define ZDIV(X,Y) ((Y==0)?(X==0?0:ZDIV_LNUM):X/Y)
#endif

#define SMALL_VALUE_ERROR_OFFSET 1e-4f

#define MAX_COVARIANCE 50

#define UNCALCULABILTY_BOUND_FOR_SAFE_EXPONENT 30.
#define SAFE_EXPONENT_MAX_VALUE 2.35385266837019985408e17f

typedef double floating_t;

typedef struct
{
    floating_t a,b,c,d;
} mat2x2;

typedef struct
{
    floating_t a,b;
} vec2;

typedef struct
{
    floating_t
    mean,
    std_dev;
} gaussian1d_t;

typedef struct
{
    vec2 mean;
    mat2x2 covariance;
    uint8_t combinations;
} gaussian2d_t;

float SafeExp(floating_t x);

void Vec2AddVec2( vec2 *, vec2 *, vec2 *);
void Vec2SubVec2( vec2 *, vec2 *, vec2 *);
void ScalarMulVec2( floating_t A, vec2 *, vec2 *);
floating_t Vec2DotVec2( vec2 *, vec2 * );
void Vec2AAT( vec2 *, mat2x2 * m );

void Mat2x2AddMat2x2( mat2x2 *, mat2x2 *, mat2x2 *);
void Mat2x2SubMat2x2( mat2x2 *, mat2x2 *, mat2x2 *);
void ScalarMulMat2x2( floating_t, mat2x2 *, mat2x2 * );
void Mat2x2DotVec2( mat2x2 *, vec2 *, vec2 *);
floating_t Mat2x2Determinant( mat2x2 * );
void Mat2x2Inverse( mat2x2 *, mat2x2 * );
void Mat2x2LLT( mat2x2 *, mat2x2 * );

void MulGaussian1d( gaussian1d_t *, gaussian1d_t *, gaussian1d_t * );
void DivGaussian1d( gaussian1d_t *, gaussian1d_t *, gaussian1d_t * );
void CopyGaussian1d( gaussian1d_t *, gaussian1d_t * );
floating_t ProbabilityFromGaussian1d( gaussian1d_t *, floating_t );

void MulGaussian2d( gaussian2d_t *, gaussian2d_t *, gaussian2d_t * );
floating_t ProbabilityFromGaussian2d( gaussian2d_t *, vec2 * );
void UpdateGaussianWithWeightGaussian2d( vec2 *, vec2 *, gaussian2d_t *, floating_t );
vec2 WeightedMeanUpdateGaussian2d( vec2 *, gaussian2d_t *, floating_t );
floating_t MahalanobisDistanceSquaredGaussian2d( mat2x2 *, vec2 * );
bool LimitCovarianceGaussian2d( mat2x2 * );
floating_t CovarianceAngleGaussian2d( mat2x2 * );

typedef struct
{
    void (*Add)( vec2 *, vec2 *, vec2 * );
    void (*Subtract)( vec2 *, vec2 *, vec2 * );
    void (*ScalarMultiply)( floating_t, vec2 *, vec2 * );
    floating_t (*Dot)( vec2 *, vec2 * );
    void (*AAT)( vec2 *, mat2x2 * );
} vec2_functions;

typedef struct
{
    void (*Add)( mat2x2 *, mat2x2 *, mat2x2 * );
    void (*Subtract)( mat2x2 *, mat2x2 *, mat2x2 * );
    void (*ScalarMultiply)( floating_t, mat2x2 *, mat2x2 * );
    void (*DotVec2)( mat2x2 *, vec2 *, vec2 * );
    floating_t (*Determinant)( mat2x2 * );
    void (*Inverse)( mat2x2 *, mat2x2 * );
    void (*LLT)( mat2x2 *, mat2x2 * );
} mat2x2_functions;

typedef struct
{
    void (*Multiply)( gaussian1d_t *, gaussian1d_t *, gaussian1d_t * );
    void (*Divide)( gaussian1d_t *, gaussian1d_t *, gaussian1d_t * );
    void (*Copy)( gaussian1d_t *, gaussian1d_t * );
    floating_t (*Probability)( gaussian1d_t *, floating_t );
} gaussian1d_functions;

typedef struct
{
    floating_t (*MahalanobisSq)( mat2x2 *, vec2 * );
    bool (*Limit)( mat2x2 * );
    floating_t (*Angle)( mat2x2 * );
} covariance_functions;
typedef struct
{
    void (*Multiply)( gaussian2d_t *, gaussian2d_t *, gaussian2d_t * );
    floating_t (*Probability)( gaussian2d_t *, vec2 * );
    void (*WeightedUpdate)( vec2 *, vec2 *, gaussian2d_t *, floating_t );
    vec2 (*WeightedMeanUpdate)( vec2 *, gaussian2d_t *, floating_t );
    covariance_functions Covariance;
} gaussian2d_functions;

typedef struct
{
    vec2_functions Vec2;
    mat2x2_functions Mat2x2;
    gaussian1d_functions Gaussian1D;
    gaussian2d_functions Gaussian2D;
} matvec_functions;

static const matvec_functions MatVec =
{
    .Vec2.Add = Vec2AddVec2,
    .Vec2.Subtract = Vec2SubVec2,
    .Vec2.ScalarMultiply = ScalarMulVec2,
    .Vec2.Dot = Vec2DotVec2,
    .Vec2.AAT = Vec2AAT,
    
    .Mat2x2.Add = Mat2x2AddMat2x2,
    .Mat2x2.Subtract = Mat2x2SubMat2x2,
    .Mat2x2.ScalarMultiply = ScalarMulMat2x2,
    .Mat2x2.DotVec2 = Mat2x2DotVec2,
    .Mat2x2.Determinant = Mat2x2Determinant,
    .Mat2x2.Inverse = Mat2x2Inverse,
    .Mat2x2.LLT = Mat2x2LLT,
    
    //    .Gaussian1D.Multiply = MulGaussian1d,
    //    .Gaussian1D.Divide = DivGaussian1d,
    //    .Gaussian1D.Copy = CopyGaussian1d,
    .Gaussian1D.Probability = ProbabilityFromGaussian1d,
    
    .Gaussian2D.Multiply = MulGaussian2d,
    .Gaussian2D.Probability = ProbabilityFromGaussian2d,
    .Gaussian2D.WeightedUpdate = UpdateGaussianWithWeightGaussian2d,
    .Gaussian2D.WeightedMeanUpdate = WeightedMeanUpdateGaussian2d,
    .Gaussian2D.Covariance.MahalanobisSq = MahalanobisDistanceSquaredGaussian2d,
    .Gaussian2D.Covariance.Limit = LimitCovarianceGaussian2d,
    .Gaussian2D.Covariance.Angle = CovarianceAngleGaussian2d
};

#endif /* matvec_h */
