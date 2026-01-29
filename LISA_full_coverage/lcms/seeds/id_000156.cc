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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsHPROFILE hProfile = NULL;
    cmsContext ctx = NULL;
    cmsIOHANDLER * io = NULL;
    cmsToneCurve * curve = NULL;
    cmsUInt16Number values[4];
    cmsUInt32Number formatter = 0;
    cmsBool md5ok = 0;

    // step 2: Setup
    hProfile = cmsCreateNULLProfile();
    ctx = cmsGetProfileContextID(hProfile);
    memset(values, 0, sizeof(values));
    values[0] = (cmsUInt16Number)0;
    values[1] = (cmsUInt16Number)16384;
    values[2] = (cmsUInt16Number)32768;
    values[3] = (cmsUInt16Number)49152;

    // step 3: Core operations
    curve = cmsBuildTabulatedToneCurve16(ctx, 4U, values);
    io = cmsOpenIOhandlerFromNULL(ctx);
    md5ok = cmsMD5computeID(hProfile);
    formatter = cmsFormatterForPCSOfProfile(hProfile, (cmsUInt32Number)128U, (cmsBool)0);

    // step 4: Cleanup
    cmsUnregisterPluginsTHR(ctx);

    (void)curve;
    (void)io;
    (void)values;
    (void)formatter;
    (void)md5ok;
    (void)ctx;
    (void)hProfile;

    // API sequence test completed successfully
    return 66;
}