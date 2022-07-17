//
//  rho_webcam.hpp
//  ots-proto
//
//  Created by Matthew Fonken on 7/16/22.
//

#ifndef rho_webcam_hpp
#define rho_webcam_hpp

#include "environment_master.hpp"
#include "rho_wrapper.hpp"
#include "webcam_utility.hpp"

#define DEBUG_RWU

#ifdef DEBUG_RWU
#define LOG_RWU(L, ...) LOG(L, "<RhoWebcamUtility> " __VA_ARGS__)
#else
#define LOG_RWU(L, ...)
#endif

class RhoWebcamUtility: public TestInterface
{
    int id;
    const std::string name;
    void init( void );
    string serialize( void );
    
    pthread_mutex_t mutex;
    cimage_t cimage;
    
    WebcamUtility cam;
    RhoWrapper rho_wrapper;
    
    rho_packet_t packet;
    
public:
    RhoWebcamUtility(string n = "rho-webcam", camera_intrinsics_t * camera_intrinsics = NULL, int id = 0, int width = -1, int height = -1);
    
    void trigger( void );
    
};

#endif /* rho_webcam_hpp */
