#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 44
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Declarations (use only available public APIs / types)
    cmsContext ctx = NULL;
    cmsUInt16Number alarmCodes[16] = {
        1000, 1001, 1002, 1003,
        1004, 1005, 1006, 1007,
        1008, 1009, 1010, 1011,
        1012, 1013, 1014, 1015
    };
    // Open a generic IO handler and keep it as a cmsHANDLE to avoid NULL handle assertions
    cmsHANDLE hIT8 = (cmsHANDLE) cmsOpenIOhandlerFromNULL(ctx);
    cmsFloat64Number sampleVal = 123.456;
    cmsBool it8res = (cmsBool) 0;

    // Step 2: Use a few profile-related APIs in a straight-line sequence
    cmsSetAlarmCodes(alarmCodes);

    // Create a minimal profile and operate on it
    cmsHPROFILE hProfile = cmsCreateNULLProfile();
    cmsUInt8Number profileID[16];
    cmsGetHeaderProfileID(hProfile, profileID);

    cmsSetProfileVersion(hProfile, 4.50);
    cmsFloat64Number profVersion = cmsGetProfileVersion(hProfile);

    cmsInt32Number tagCount = cmsGetTagCount(hProfile);

    // Open another IO handler and save the profile into it
    cmsIOHANDLER * io = cmsOpenIOhandlerFromNULL(ctx);
    cmsUInt32Number bytesNeeded = cmsSaveProfileToIOhandler(hProfile, io);

    // Query supported intents (THR variant) with small arrays
    cmsUInt32Number intents[8];
    char * descriptions = NULL;
    cmsUInt32Number nIntents = cmsGetSupportedIntentsTHR(ctx, 8U, intents, &descriptions);

    // Use a couple more informational APIs
    cmsColorSpaceSignature pcs = cmsGetPCS(hProfile);
    cmsProfileClassSignature devClass = cmsGetDeviceClass(hProfile);
    cmsUInt32Number renderIntent = cmsGetHeaderRenderingIntent(hProfile);

    // Step 3: Silence unused variable warnings and finish
    (void) hIT8;
    (void) sampleVal;
    (void) it8res;
    (void) profVersion;
    (void) tagCount;
    (void) bytesNeeded;
    (void) nIntents;
    (void) pcs;
    (void) devClass;
    (void) renderIntent;
    (void) io;
    (void) descriptions;
    (void) hProfile;
    (void) ctx;

    return 66;
}