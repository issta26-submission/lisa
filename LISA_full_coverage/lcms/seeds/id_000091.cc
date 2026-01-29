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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsToneCurve * tone = NULL;
    cmsUInt16Number table[4];
    cmsBool isMono = 0;
    cmsUInt8Number profileID[16];
    cmsFloat64Number version = 4.5;
    cmsHTRANSFORM xform = NULL;
    cmsHPROFILE devlink = NULL;
    cmsContext pipeCtx = NULL;

    // step 2: Setup / Initialize objects
    ctx = cmsCreateContext(NULL, NULL);
    table[0] = (cmsUInt16Number)0;
    table[1] = (cmsUInt16Number)16384;
    table[2] = (cmsUInt16Number)49152;
    table[3] = (cmsUInt16Number)65535;
    tone = cmsBuildTabulatedToneCurve16(ctx, 4U, table);
    hProfile = cmsCreate_sRGBProfileTHR(ctx);
    memset(profileID, 0, sizeof(profileID));
    cmsSetProfileVersion(hProfile, version);
    cmsGetHeaderProfileID(hProfile, profileID);

    // step 3: Operate / Transform and Validate
    isMono = cmsIsToneCurveMonotonic(tone);
    xform = cmsCreateTransformTHR(ctx, hProfile, 0U, hProfile, 0U, 0U, 0U);
    devlink = cmsTransform2DeviceLink(xform, version, 0U);
    pipeCtx = cmsDupContext(ctx, NULL); // obtain a valid context instead of passing NULL to cmsGetPipelineContextID
    cmsSetProfileVersion(devlink, version + (cmsFloat64Number)isMono);

    // step 4: Cleanup
    cmsDeleteTransform(xform);
    xform = NULL;
    tone = NULL;
    hProfile = NULL;
    devlink = NULL;
    ctx = pipeCtx;
    (void)table;
    (void)profileID;
    (void)version;
    (void)isMono;

    return 66;
    // API sequence test completed successfully
}