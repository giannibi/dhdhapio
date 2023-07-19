/**-----------------------------------------------------------------------------------
 *  Copyright (C) 2014 Gianni Bianchini
 *
 *
 *  This file is part of HART, the Hardware Access in Real Time Toolbox for Scilab/Scicos.
 *
 *  HART is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  HART is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with HART; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *--------------------------------------------------------------------------------- */


#include <machine.h>
#include <scicos_block4.h>

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dhdc.h"

// Assume for now that each device has 3 DOF
#define NDOF 3
#define DEBUG

#include "hart_hapio.h"

#define p_numDevices block->ipar[0]


#ifdef __cplusplus
extern "C" {
#endif



static void init(scicos_block *block)
{
    int i;

    numDevices = dhdGetDeviceCount();
	

#ifdef DEBUG
    fprintf(stderr,"*** DHD debug *** Declared %d devices\n",p_numDevices);
    fprintf(stderr,"*** DHD debug *** Detected %d devices\n",numDevices);
    if (numDevices != p_numDevices) {
       fprintf(stderr,"*** DHD debug *** WARNING: device number mismatch\n");
    }
#endif

    deviceID = (int*) calloc(numDevices, sizeof(int));

    for(i = 0; i < numDevices; i++) {
      deviceID[i] = dhdOpenID(i);
     dhdSetForce(0,0,0,deviceID[i]);
    } 
 
}

static void inout(scicos_block *block) {

  double *y = GetRealOutPortPtrs(block,1);
  double *u = GetRealInPortPtrs(block,1);
   

int i;
  
        // Get position for each device and 
  
        for(i = 0; i < numDevices; i++) {
          dhdGetPosition(&y[NDOF*i], &y[NDOF*i+1], &y[NDOF*i+2], deviceID[i]);
     

#ifdef DEBUG2
        fprintf(stderr,"*** DHD debug *** D%d: x=%f y=%f z=%f\n",i,y[NDOF*i],y[NDOF*i+1],y[NDOF*i+2]);
#endif
        }


        
       
        // Set forces for each device
        
        for(i = 0; i < numDevices; i++) {
          dhdSetForce(u[NDOF*i], u[NDOF*i+1], u[NDOF*i+2], deviceID[i]);
        }
        
        
}


static void end(scicos_block *block) {

  int i;


  // Reset forces and close devices
   
  for(i = 0; i < numDevices; i++) {

#ifdef DEBUG
  printf("*** DHD debug *** Closing device %d\n", i);
#endif
    
      dhdClose(i);
  }

  // Free memory
  
  free(deviceID);
}


void rt_hapio_device(scicos_block *block,int flag)
{
  if (flag==1){          /* set output */
    inout(block);
  }
  else if (flag==5){     /* termination */ 
    end(block);
  }
  else if (flag ==4){    /* initialisation */
    init(block);
  }
}

#ifdef __cplusplus
}
#endif

