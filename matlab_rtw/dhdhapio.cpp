/* dhdhapio.c: chai3d to Simulink/RTW for haptic device I/O - S-Function
              (C) 2011 Gianni Bianchini <giannibi@dii.unisi.it>
*/

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  dhdhapio

#include "simstruc.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dhdc.h"

// Definition of parameters from the block mask
#define NPARAMS 1
#define p_declarednumdevices(S)                ssGetSFcnParam(S,0)


// Activate debug messages
//#define DEBUG

// Activate verbose debug messages
//#define DEBUG2

// Activate data dump
//#define DEBUG3

// Assume for now that each device has 3 DOF
#define NDOF 3

// mdlInitializeSizes ===============================================

static void mdlInitializeSizes(SimStruct *S)
{

#ifdef DEBUG
    printf("DHD to Simulink/RTW interface for haptic device I/O\n(C) 2011 Gianni Bianchini <giannibi@dii.unisi.it>\n");
#endif

#ifdef DEBUG2
    printf("Entering mdlInitializeSizes\n");
#endif
 
    // Declare number of parameters
    ssSetNumSFcnParams(S, NPARAMS);

    // Parameter mismatch will be reported by Simulink
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        return;
    }

    // Retrieve parameters
    // Number of devices
    int_T declarednumdevices = *mxGetPr(p_declarednumdevices(S));

#ifdef DEBUG
    printf("This model is designed for %d devices\n", declarednumdevices);
#endif


   /* No continuous states */

    ssSetNumContStates(S, 0);

   /* No discrete states */
   
    ssSetNumDiscStates(S, 0);     

 
    // And integer parameters
    ssSetNumIWork(S, 1);
    ssSetNumPWork(S, 1);

    // Specify I/O (NDOF ports for each device for now)
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, NDOF * declarednumdevices);

    if (!ssSetNumOutputPorts(S,1)) return;
    ssSetOutputPortWidth(S, 0, NDOF * declarednumdevices);

    // Specify sample times
    ssSetNumSampleTimes(S, 1);

    //ssSetSimStateCompliance(S, USE_CUSTOM_SIM_STATE);

    ssSetOptions(S,
              //   SS_OPTION_WORKS_WITH_CODE_REUSE |
                 SS_OPTION_EXCEPTION_FREE_CODE);

#ifdef DEBUG2
    printf("Exiting mdlInitializeSizes\n");
#endif

}


// mdlInitializeSampleTimes =========================================

static void mdlInitializeSampleTimes(SimStruct *S)
{
#ifdef DEBUG2
    printf("Entering mdlInitializeSampleTimes\n");
#endif
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S); 
#ifdef DEBUG2
    printf("Exiting mdlInitializeSampleTimes\n");
#endif
}

// mdlStart =======================================================

#define MDL_START
static void mdlStart(SimStruct *S)
{
#ifdef DEBUG2
    printf("Entering mdlStart\n");
#endif


    int numDevices;
    int i;

    numDevices = dhdGetDeviceCount();



#ifdef DEBUG
    printf("Detected %d devices\n",numDevices);
    int_T declarednumdevices = *mxGetPr(p_declarednumdevices(S));
    if (numDevices != declarednumdevices) {
       printf("WARNING: device number mismatch\n");
    }
#endif


//#ifndef RT
//    if (numDevices <=0 ) {
//        ssSetErrorStatus(S, "No devices found");
//        return;
//    }
//#endif


   int* deviceID = (int*) calloc(numDevices, sizeof(int));


    i = 0;
    while (i < numDevices)
    {

    deviceID[i] = dhdOpenID(i);
    dhdSetForce(0,0,0,deviceID[i]);

        i++;
    }

     // Store actual number of devices in IW 0
     ssGetIWork(S)[0]=numDevices;

     ssGetPWork(S)[0] = (void *) deviceID;

#ifdef DEBUG2
  printf("Exiting mdlStart\n");
#endif

}


// mdlUpdate ==============================================================

#define MDL_UPDATE
static void mdlUpdate(SimStruct *S, int_T tid)
{
  
    int *deviceID = static_cast<int *>(ssGetPWork(S)[0]);
    int numDevices = ssGetIWork(S)[0];

    // Get data addresses of inputs
    InputRealPtrsType u = ssGetInputPortRealSignalPtrs(S,0);


 // for each device
        int i=0;
        while (i < numDevices)
        { 
         
        
            // send force to haptic device
            dhdSetForce (*u[NDOF*i], *u[NDOF*i+1], *u[NDOF*i+2], deviceID[i]);

            i++;
        }
}



// mdlOutputs =======================================================

static void mdlOutputs(SimStruct *S, int_T tid)
{


    int *deviceID = static_cast<int *>(ssGetPWork(S)[0]);
    int numDevices = ssGetIWork(S)[0];

    // Get data addresses of outputs
   real_T *y = ssGetOutputPortRealSignal(S, 0);

 // for each device
        int i=0;
        while (i < numDevices)
        {
            // read position of haptic device

         dhdGetPosition (&y[NDOF*i], &y[NDOF*i+1], &y[NDOF*i+2], deviceID[i]);


#ifdef DEBUG
        fprintf(stderr,"D%d: x=%f y=%f z=%f\n",i,y[NDOF*i],y[NDOF*i+1],y[NDOF*i+2]);
#endif

            // read orientation of haptic device
        //    cMatrix3d newRotation;
         //   hapticDevices[i]->getRotation(newRotation);


            // read linear velocity from device
         //   cVector3d linearVelocity;
         //   hapticDevices[i]->getLinearVelocity(linearVelocity);

            // read user button status
     //       bool buttonStatus;
       //     hapticDevices[i]->getUserSwitch(0, buttonStatus);

       
            i++;
        }
}


// mdlTerminate =====================================================

static void mdlTerminate(SimStruct *S)
{
#ifdef DEBUG2
    printf("Entering mdlTerminate\n");
#endif

     int *deviceID = static_cast<int *>(ssGetPWork(S)[0]);
    int numDevices = ssGetIWork(S)[0];


  // Reset forces and close devices
  int i=0;  
  while (i < numDevices) {

#ifdef DEBUG
  printf("Closing device %d\n", i);
#endif
      dhdClose(i);
         i++;
  }

  // Free memory
   free(deviceID);


#ifdef DEBUG2
    printf("Exiting mdlTerminate\n");
#endif

}


// Required S-function trailer
#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
