/************************************************************************
 *  File: rho_client.h
 *  Group: Rho Core
 ***********************************************************************/

#ifndef APPLICATION_USER_RHO_CORE_RHO_CAPTURE_H_
#define APPLICATION_USER_RHO_CORE_RHO_CAPTURE_H_

/************************************************************************
 *                             Includes                                 *
 ***********************************************************************/

#include "rho_core.h"

#ifdef __STOPWATCH__
#include "stopwatch.h"
#endif


//#define __ASSEMBLY_RHO__

//#define CAPTURE_WIDTH 100
//#define MIN(A, B) (A < B ? A : B)

#define MAX_BLOBS 3

typedef struct
{
    density_2d_t left, right;       /* Direction density values */
    index_t thresh_proc, rows_proc;
    bool complete;
} section_process_t;

typedef struct
{
    index_t x, y;
    index_t w, h;
} blob_t;

typedef struct
{
    index_t i;
    byte_t id;
    bool open;
} edge_t;

typedef struct
{
    byte_t edge_proc;
    byte_t num_blobs;
    byte_t num_active;
    edge_t edges[MAX_BLOBS * 2];
    byte_t edge_order[MAX_BLOBS * 2];
    blob_t blobs[MAX_BLOBS];
    index_t thresh_blob_loc[MAX_BLOBS];
    bool blob_i_active[MAX_BLOBS];
} rho_capture_t;

index_t * RhoCapture_CaptureRow( const byte_t * capture_address,
                              const byte_t thresh_value,
                              index_t * thresh_address,
                              const index_t length,
                              byte_t subsamble );
void RhoCapture_CaptureBlobs( rho_capture_t * _,
                              index_t row,
                              const byte_t * capture_address,
                              const byte_t thresh_value,
                                index_t * thresh_address,
                              const index_t length,
                              byte_t sub_sample );
section_process_t RhoCapture_ProcessFrameSection( const index_t rows,
                              index_t * thresh_address,
                              const index_t * thresh_end,
                              const density_t Cx,
                              sdensity_t * Dy,
                              sdensity_t * Dx_i );
void RhoCapture_OrderEdges( blob_t * blobs, byte_t n, edge_t * edges, byte_t * order, index_t max_i );
void RhoCapture_AssignBlobsInThreshBuffer( index_t *thresh_blob_loc, blob_t * blob, byte_t n, edge_t * edges, byte_t * order);
void RhoCapture_PrepareBlobsForCapture( rho_capture_t * _, index_t max_y );
void RhoCapture_AddBlob( rho_capture_t * _, blob_t blob );
void RhoCapture_ResetEdges( rho_capture_t * _ );
void RhoCapture_ResetBlobs( rho_capture_t * _ );
void RhoCapture_ResetAll( rho_capture_t * _ );

typedef struct
{
    index_t * (*CaptureRow)( const byte_t *, const byte_t, index_t *, index_t, byte_t );
    void (*CaptureBlobs)( rho_capture_t *, index_t, const byte_t *, const byte_t, index_t *, const index_t, byte_t );
    section_process_t (*ProcessFrameSection)( const index_t, index_t *, const index_t *, const density_t, sdensity_t *, sdensity_t *);
    void (*OrderEdges)( blob_t * blobs, byte_t n, edge_t * edges, byte_t * order, index_t max_i );
    void (*AssignBlobsInThreshBuffer)( index_t *thresh_blob_loc, blob_t * blob, byte_t n, edge_t * edges, byte_t * order);
    void (*PrepareBlobsForCapture)( rho_capture_t * _, index_t max_y );
    void (*AddBlob)( rho_capture_t * _, blob_t blob );
    void (*ResetEdges)( rho_capture_t * _ );
    void (*ResetBlobs)( rho_capture_t * _ );
    void (*ResetAll)( rho_capture_t * _ );
} rho_capture_functions;

extern rho_capture_functions RhoCapture;

#endif /* APPLICATION_USER_RHO_CORE_RHO_CAPTURE_H_ */
