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
//<ID> 154
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
    cmsBool md5ok = 0;
    cmsUInt32Number formatter = 0;
    cmsUInt32Number savedBytes = 0;
    cmsUInt16Number values[4];
    memset(values, 0, sizeof(values));

    // step 2: Setup
    hProfile = cmsCreateNULLProfile();
    ctx = cmsGetProfileContextID(hProfile);
    io = cmsOpenIOhandlerFromNULL(ctx);
    values[0] = 0u;
    values[1] = 0x4000u;
    values[2] = 0x8000u;
    values[3] = 0xFFFFu;
    curve = cmsBuildTabulatedToneCurve16(ctx, 4u, values);

    // step 3: Operate and validate
    formatter = cmsFormatterForPCSOfProfile(hProfile, 32u, (cmsBool)0);
    md5ok = cmsMD5computeID(hProfile);
    savedBytes = cmsSaveProfileToIOhandler(hProfile, io);

    // step 4: Cleanup
    memset(values, 0, sizeof(values));
    (void)curve;
    (void)ctx;
    (void)io;
    (void)formatter;
    (void)md5ok;
    (void)savedBytes;

    // API sequence test completed successfully
    return 66;
}