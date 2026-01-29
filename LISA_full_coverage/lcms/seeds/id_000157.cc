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
//<ID> 157
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
    cmsToneCurve * curve = NULL;
    cmsIOHANDLER * io = NULL;
    cmsUInt16Number values[4];
    cmsUInt32Number nEntries = 4;
    cmsUInt32Number fmt = 0;
    cmsUInt32Number bytesSaved = 0;
    cmsBool md5ok = 0;

    // step 2: Setup
    hProfile = cmsCreateNULLProfile();
    ctx = cmsGetProfileContextID(hProfile);
    memset(values, 0, sizeof(values));
    values[0] = 0;
    values[1] = 16384U;
    values[2] = 49152U;
    values[3] = 65535U;
    curve = cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
    io = cmsOpenIOhandlerFromNULL(ctx);

    // step 3: Core operations
    bytesSaved = cmsSaveProfileToIOhandler(hProfile, io);
    fmt = cmsFormatterForPCSOfProfile(hProfile, 128U, 0);
    md5ok = cmsMD5computeID(hProfile);

    // step 4: Validate and Cleanup
    (void)curve;
    (void)bytesSaved;
    (void)fmt;
    (void)md5ok;
    (void)io;
    (void)ctx;
    (void)values;
    (void)nEntries;
    (void)hProfile;

    return 66;
    // API sequence test completed successfully
}