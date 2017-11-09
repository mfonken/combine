/*! \file centroid.h
 \brief Centroid Detection
 
 Detects object persistence from each line input
 one at a time and returns the centroid coordinates
 for each object.
 */

#ifndef centroidv3_h
#define centroidv3_h

#define MAX_CENTROIDS   100
#define MAP_SIZE        MAX_CENTROIDS * 2
#define MAX_SEGMENTS    400
#define MAX_GAP         1

//#define DEBUG

using namespace std;
typedef struct
{
    double  X;                      /**< X center value */
    double  Y;                      /**< Y center value */
    int     M;
    int     n;
} centroid_t;

typedef struct
{
    int     i;
    int     l;
    double  x;
    double  w;
    int     m;
} segment_t;

int map_index = 0;
int segment_index = 0;

typedef struct
{
    int     p; // point
    bool    a; // active
    int     y; // last y
    double  x; // last x
    int     w; // last width
} map_t;

map_t map[MAP_SIZE];
segment_t segments[MAX_SEGMENTS];
centroid_t centroids[MAX_CENTROIDS];  /**< Global array of detected object */
double a_x_last = 0;                /**< Global previous X average */

void printS( segment_t * s )
{
    printf("Segment:  (%d, %d, %.3f, %.3f)\n", s->i, s->l, s->x, s->w );
}

void cmav3(double new_val, double *avg, int num)
{
    *avg += (new_val - *avg)/num;
}

int processCentroids( void )
{
    int n = 0;
    printf("\nFound %d segments and used %d maps.\n", segment_index, map_index);
    for( int j = 0; j < segment_index; j++ )
    {
        segment_t s = segments[j];
        for( int i = 0; i < map_index; i++ )
        {
            if( s.i == i )
            {
                int p = map[i].p;
                n = p;
                
#ifdef DEBUG
                printf("Computing segment - %d:(%d, %.3f, %.3f) into centroid index %d\n", s.i, s.l, s.x, s.w, p);
#endif
                centroids[p].M += s.w;
                centroids[p].n++;
                cmav3( s.x, &centroids[p].X, centroids[p].n);
                /* Quick Y estimate */
                cmav3( s.l, &centroids[p].Y, centroids[p].n);
#ifdef DEBUG
                printf("Centroid %d - m%d(%.3f, %.3f)\n", p, centroids[p].M, centroids[p].X, centroids[p].Y );
#endif
            }
        }
    }
    return map_index;
}

int getSegmentId(int y, double x, int w )
{
    /* Null id (no adjacents) */
    int id = -1;
    int i, j;
    w /= 2;
    for(i = 0; i < map_index; i++)
    {
        if( map[i].a ) // Ensure map is still active
        {
#ifdef DEBUG
            printf("Map %d is active - (y%d, x%.3f, w%d)   %d\n", i, map[i].y, map[i].x, map[i].w, y);
#endif
            if( ( y - map[i].y ) > MAX_GAP)
            {
                map[i].a = false;
            }
            else
            {
#ifdef DEBUG
                printf("Comparing ( %d, %.3f, %d)\n", y, x, w);
                printf("         &( %d, %.3f, %d)\n", map[i].y, map[i].x, map[i].w );
#endif
                
                /* Get current average and previous row width */
                double x_l = map[i].x;
                double w_l = map[i].w;
                /* Check overlap of lower bound of centroid and upper (with gap tolerance) of new
                 and of upper bound of centroid and lower (with gap tolerance) of new */
                if( ( ( x + w + MAX_GAP ) >= ( x_l - w_l ) ) &&
                    ( ( x - w - MAX_GAP ) <= ( x_l + w_l ) ) )
                {
#ifdef DEBUG
                    printf("\tSegments match:\n");
#endif
                    /* If object is unclaimed, claim it, otherwise combine it */
                    if( id == -1 )
                    {
#ifdef DEBUG
                        printf("\t\tClaimed as %d\n", i);
#endif
                        id = i;
                    }
                    else
                    {
                        // NOTE: Merged maps will re-merge every time (more efficient than checking)
#ifdef DEBUG
                        printf("\t\tMerging map %d>%d to %d>%d\n", id, map[id].p, i, map[i].p);
#endif
                        map[i] = map[id];
#ifdef DEBUG
                        printf("\t\tMerged map %d>%d to %d>%d\n", id, map[id].p, i, map[i].p);
#endif
                    }
                }
            }
        }
    }
    /* Valid if object was claimed, null if not */
    return id;
}

/*! Main Centroid Detection
 */
int getCentroidsv3(char *image_line, int line_length, int line_number )
{
    /* Global variables */
    int gap = -1, num_adj = 0,  temp_id = 0, x;
    
    /* Traverse all columns */
    for(x = 0; x < line_length; x++)
    {
        /* Check if pixel is on */
        if(image_line[x])
        {
            /* Reset gap counter, add to average of adjacent pixels, and increment */
            gap = 0;
            cmav3(x, &a_x_last, num_adj + 1);
            num_adj++;
        }
        /* Otherwise, if gap counter is counting (i.e. there was a recent pixel) */
        else if(gap != -1)
        {
            /* Check if the max gap reached or the row has ended */
            gap++;
            if( gap ==  MAX_GAP )
            {
#ifdef DEBUG
                printf("Getting segment id at line #%d\n", line_number);
#endif
                temp_id = getSegmentId( line_number, a_x_last, num_adj );
#ifdef DEBUG
                printf("\t:%d\n", temp_id);
#endif
                if(temp_id == -1)
                {
                    temp_id = map_index++;
                    map[temp_id].p = temp_id;
                    map[temp_id].a = true;
                }
                map[temp_id].x = a_x_last;
                map[temp_id].w = num_adj / 2;
                map[temp_id].y = line_number;
                segments[segment_index].i = map[temp_id].p;
                segments[segment_index].l = line_number;
                segments[segment_index].x = a_x_last;
                segments[segment_index].w = num_adj;
#ifdef DEBUG
                printS(&segments[segment_index]);
#endif
                segment_index++;
                num_adj = 0;            // Reset number of adjacent pixels
                a_x_last = 0;           // Reset adjacent pixel average
                gap = -1;
            }
        }
    }
    
    return segment_index;
}

void resetCentroids( void )
{
    segment_index = 0;
    map_index = 0;
    a_x_last = 0;
    
    for( int i = 0; i < MAX_CENTROIDS; i++ )
    {
        centroids[i].n = 0;
    }
}

#endif /* centroid_h */
