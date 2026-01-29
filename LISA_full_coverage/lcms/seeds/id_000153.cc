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
//<ID> 153
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
    cmsUInt16Number table[5];
    cmsToneCurve * curve = NULL;
    cmsUInt16Number evalVal = 0;
    cmsBool md5ok = 0;
    cmsUInt32Number written = 0;
    cmsUInt32Number fmt = 0;
    cmsFloat64Number gamma = 0.0;

    // step 2: Setup
    hProfile = cmsCreateNULLProfile();
    ctx = cmsGetProfileContextID(hProfile);
    io = cmsOpenIOhandlerFromNULL(ctx);
    table[0] = 0;
    table[1] = 16384;
    table[2] = 32768;
    table[3] = 49152;
    table[4] = 65535;
    curve = cmsBuildTabulatedToneCurve16(ctx, 5U, table);

    // step 3: Core operations (initialize -> configure -> operate -> validate)
    evalVal = cmsEvalToneCurve16(curve, 32768U);
    md5ok = cmsMD5computeID(hProfile);
    written = cmsSaveProfileToIOhandler(hProfile, io);
    fmt = cmsFormatterForPCSOfProfile(hProfile, 32U, 0); /* not float */
    gamma = cmsEstimateGamma(curve, 0.01);

    // step 4: Cleanup
    cmsUnregisterPluginsTHR(ctx);
    cmsSetLogErrorHandlerTHR(ctx, NULL);
    memset(table, 0, sizeof(table));
    hProfile = NULL;
    io = NULL;
    curve = NULL;

    (void)evalVal;
    (void)md5ok;
    (void)written;
    (void)fmt;
    (void)gamma;
    (void)ctx;

    // API sequence test completed successfully
    return 66;
}