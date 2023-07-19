/* hapio.cpp: chai3d to Simulink/RTW for haptic device I/O - S-Function
              (C) 2011 Gianni Bianchini <giannibi@dii.unisi.it>
*/

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  hapio

#define _LINUX
#define dDOUBLE

#include "simstruc.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chai3d.h"

// Definition of parameters from the block mask
#define NPARAMS 1
#define p_declarednumdevices(S)                ssGetSFcnParam(S,0)


// Activate debug messages
#define DEBUG

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
    printf("\nchai3d to Simulink/RTW interface for haptic device I/O\n(C) 2011 Gianni Bianchini <giannibi@dii.unisi.it>\n");
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

    // Reserve place for work parameter pointers
    // PW 0: Haptic device handler
    // PW 1: Device array
    ssSetNumPWork(S, 2);
    // And integer parameters
    ssSetNumIWork(S, 1);

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

    // Declare device handler
    cHapticDeviceHandler *handler;
    // Declare device object array
    cGenericHapticDevice** hapticDevices;

    int numDevices;
    int i;

    // Set up a device handler
    handler = new cHapticDeviceHandler();
    numDevices = handler->getNumDevices();

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

    // Store the device handler pointer in PW 0
    ssGetPWork(S)[0] = (void *)handler;

    // Allocate haptic device pointer vector
    hapticDevices = (cGenericHapticDevice**)(calloc(numDevices, sizeof(cGenericHapticDevice*)));
    // and store it in PW 1
    ssGetPWork(S)[1] = (void *)hapticDevices;

    i = 0;
    while (i < numDevices)
    {
        // get a handle to the i-th haptic device
        cGenericHapticDevice* newHapticDevice;        
        handler->getDevice(newHapticDevice, i);
        cHapticDeviceInfo info = newHapticDevice->getSpecifications();

#ifdef DEBUG
    char *namestr = new char [info.m_modelName.size()+1];
    strcpy (namestr, info.m_modelName.c_str());
    printf("Device #%d is a %s\n",i,namestr);
    delete[] namestr;
    printf("Opening device %d\n",i);
#endif

        // open connection to haptic device
        newHapticDevice->open();
		
        // initialize haptic device
        newHapticDevice->initialize();

        // store the handle in the haptic device array
        hapticDevices[i] = newHapticDevice;

        i++;
    }

     // Store actual number of devices in IW 0
     ssGetIWork(S)[0]=numDevices;

#ifdef DEBUG2
  printf("Exiting mdlStart\n");
#endif

}


// mdlUpdate ==============================================================

#define MDL_UPDATE
static void mdlUpdate(SimStruct *S, int_T tid)
{
  // Retrieve C++ object from the pointers vector
    cHapticDeviceHandler *handler = static_cast<cHapticDeviceHandler *>(ssGetPWork(S)[0]);   
    cGenericHapticDevice **hapticDevices = static_cast<cGenericHapticDevice **>(ssGetPWork(S)[1]); 

    int numDevices = ssGetIWork(S)[0];

    // Get data addresses of inputs
    InputRealPtrsType u = ssGetInputPortRealSignalPtrs(S,0);


 // for each device
        int i=0;
        while (i < numDevices)
        { 
            // retrieve forces from inputs
            cVector3d force (*u[NDOF*i], *u[NDOF*i+1], *u[NDOF*i+2]);
            
            // send force to haptic device
            hapticDevices[i]->setForce(force);

            i++;
        }
}



// mdlOutputs =======================================================

static void mdlOutputs(SimStruct *S, int_T tid)
{
    cVector3d position;

    // Retrieve C++ object from the pointers vector
    cHapticDeviceHandler *handler = static_cast<cHapticDeviceHandler *>(ssGetPWork(S)[0]);   
    cGenericHapticDevice **hapticDevices = static_cast<cGenericHapticDevice **>(ssGetPWork(S)[1]); 

    int numDevices = ssGetIWork(S)[0];

    // Get data addresses of outputs
   real_T *y = ssGetOutputPortRealSignal(S, 0);

 // for each device
        int i=0;
        while (i < numDevices)
        {
            // read position of haptic device
            hapticDevices[i]->getPosition(position);

            // write positon to output
            y[NDOF*i] = position.x;
            y[NDOF*i+1]=position.y;
            y[NDOF*i+2]=position.z;

#ifdef DEBUG3
        printf("D%d: x=%f y=%f z=%f\n",i,y[NDOF*i],y[NDOF*i+1],y[NDOF*i+2]);
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

  // Retrieve C++ object from the pointers vector
  cHapticDeviceHandler *handler = static_cast<cHapticDeviceHandler *>(ssGetPWork(S)[0]);   
  cGenericHapticDevice **hapticDevices = static_cast<cGenericHapticDevice **>(ssGetPWork(S)[1]);

  int numDevices = ssGetIWork(S)[0];

  // Reset forces and close devices
  int i=0;  
  while (i < numDevices) {

#ifdef DEBUG
  printf("Closing device %d\n", i);
#endif
         cVector3d force (0,0,0);
         hapticDevices[i]->setForce(force);
         hapticDevices[i]->close();
         i++;
  }

  // Free memory
    delete handler;
    free(hapticDevices); 

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
