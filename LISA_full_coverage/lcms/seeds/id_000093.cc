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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE srgb = NULL;
    cmsHPROFILE devlink = NULL;
    cmsToneCurve * tone = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsUInt16Number table[4];
    cmsUInt8Number profileID[16];
    cmsFloat64Number version = 4.3;
    cmsBool monotonic = 0;
    cmsPipeline * pipeline = NULL;
    cmsContext pipeCtx = NULL;
    cmsUInt32Number bytesNeeded = 0U;
    cmsBool saved = 0;

    // step 2: Setup
    ctx = cmsCreateContext(NULL, NULL);
    srgb = cmsCreate_sRGBProfileTHR(ctx);
    table[0] = (cmsUInt16Number)0;
    table[1] = (cmsUInt16Number)16384;
    table[2] = (cmsUInt16Number)49152;
    table[3] = (cmsUInt16Number)65535;
    tone = cmsBuildTabulatedToneCurve16(ctx, 4U, table);
    memset(profileID, 0, sizeof(profileID));

    // step 3: Core operations (Initialize -> Configure -> Operate -> Validate)
    monotonic = cmsIsToneCurveMonotonic(tone);
    cmsSetProfileVersion(srgb, version);
    cmsGetHeaderProfileID(srgb, profileID);
    xform = cmsCreateTransformTHR(ctx, srgb, 0U, srgb, 0U, 0U, 0U);
    devlink = cmsTransform2DeviceLink(xform, version, 0U);
    pipeCtx = cmsGetTransformContextID(xform);
    saved = cmsSaveProfileToMem(devlink, NULL, &bytesNeeded);

    // step 4: Cleanup
    cmsDeleteTransform(xform);
    xform = NULL;
    devlink = NULL;
    srgb = NULL;
    tone = NULL;
    (void)ctx;
    (void)monotonic;
    (void)profileID;
    (void)version;
    (void)pipeCtx;
    (void)bytesNeeded;
    (void)saved;
    // API sequence test completed successfully
    return 66;
}