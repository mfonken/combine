//
//  main.c
//  C11_rtos
//
//  Created by Matthew Fonken on 8/5/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <stdio.h>

#include "systembridge.h"

//#define COMPONENT_ID(A,B) ( ( A << 8 ) & 0xff00 | ( B & 0x00ff ) )
//#define ID(A,B) { A, B }
//typedef struct
//{
//    component_id id;
//} id_test_t;

int main(int argc, const char * argv[])
{
//    uint16_t a = COMPONENT_ID( 1, 2);
//    id_test_t b = ID(1,2);
//    printf("%04x: %02x|%02x\n", b.id, b.id.macro, b.id.micro);
    InitializeMeta();
    return 0;
}
