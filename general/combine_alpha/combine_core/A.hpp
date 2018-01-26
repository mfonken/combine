/*******************************************************************
 * FILE:        A.hpp
 * AUTHOR:      Matthew Fonken on 1/25/18.
 * DESCRIPTION: Example implementation of the TestInterface for
 *              using my testing Environment structure.
*******************************************************************/

#ifndef A_hpp
#define A_hpp

/* Standard Include */
#include <stdio.h>

/* Environment Include */
#include "environment_master.h"

/* TestInterface that looks like:
 *
 * class TestInterface
 * {
 *  public:
 *      int id;
 *      std::string name;
 *      virtual void  init( void ) = 0;
 *      virtual void  trigger( void ) = 0;
 *      virtual string serialize( void ) = 0;
 * };
 *
 * NOTE: The "init" is designed to be different from a constructor and is
 *       suggested for most hardware implementations for performing startup
 *       routines. It is automatically called when the event is added to the
 *       Environment either by the Environment's constructor or by addTest().
 */

class A : public TestInterface
{
    /* Interface Concretions */
public:
    int id;
    std::string name;
    void  init( void ) {  printf("Initializing test with name \"%s\" and id %d.\n", name.c_str(), id);  };
    void  trigger( void ) { /* printf("Triggering with counter=%d", counter++);  printf("%d-%d\n", id, counter++); */};
    string serialize( void ) { /* printf("Serializing counter=%d", counter); */ return name; };
    
    /* Example Constructors */
    A() { id = 0; counter = 0; };
    A(int i) { id = i; counter = 0; };
    A(std::string n, int i) { name = n; id = i; counter = 0; };
    
    /* Example Variables */
    int counter;
};

#endif /* A_hpp */
