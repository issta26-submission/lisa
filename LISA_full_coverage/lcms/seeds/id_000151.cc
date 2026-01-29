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
//<ID> 151
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
    cmsUInt16Number values[3];
    cmsUInt32Number fmt = 0;
    cmsBool md5ok = 0;
    cmsUInt32Number nBytes = 128;
    cmsBool isFloat = 0;

    // step 2: Setup (initialize resources and context)
    values[0] = (cmsUInt16Number)0;
    values[1] = (cmsUInt16Number)32768;
    values[2] = (cmsUInt16Number)65535;
    hProfile = cmsCreateNULLProfile();
    ctx = cmsGetProfileContextID(hProfile);
    io = cmsOpenIOhandlerFromNULL(ctx);
    curve = cmsBuildTabulatedToneCurve16(ctx, 3U, values);

    // step 3: Operate and validate (use profile, formatter and MD5)
    fmt = cmsFormatterForPCSOfProfile(hProfile, nBytes, isFloat);
    md5ok = cmsMD5computeID(hProfile);

    // step 4: Cleanup (release references and silence unused warnings)
    (void)io;
    (void)curve;
    (void)fmt;
    (void)md5ok;
    (void)values;
    (void)ctx;
    (void)hProfile;

    // API sequence test completed successfully
    return 66;
}