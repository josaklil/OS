/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: trace
 *
 * Created on October 24, 2018, 11:00 PM
 */

#include <cstdlib>
#include <MMU.h>
#include "Process.h"
#include "MemoryAllocator.h"

using namespace mem;

/*
 * 
 */
int main(int argc, char** argv) {

    //MMU mem(256);
    
    MemoryAllocator(256);
    
    return 0;
}

