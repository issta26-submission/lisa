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
//<ID> 352
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsHPROFILE profiles[2];
    cmsHPROFILE prof_srgb = cmsCreate_sRGBProfile();
    cmsHPROFILE prof_xyz  = cmsCreateXYZProfile();
    profiles[0] = prof_srgb;
    profiles[1] = prof_xyz;
    cmsHTRANSFORM xform = NULL;
    cmsContext ctx = NULL;
    cmsFloat64Number prevAdapt = 0.0;
    cmsUInt32Number inFormat = 0;
    cmsColorSpaceSignature pcs = (cmsColorSpaceSignature)0;
    cmsHPROFILE devlink = NULL;
    cmsFloat64Number adaptValue = 1.0;

    // step 2: Setup
    xform = cmsCreateMultiprofileTransformTHR(NULL, profiles, (cmsUInt32Number)2, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 3: Core operations
    ctx = cmsGetTransformContextID(xform);
    prevAdapt = cmsSetAdaptationStateTHR(ctx, adaptValue);
    inFormat = cmsGetTransformInputFormat(xform);
    pcs = cmsGetPCS(prof_srgb);
    devlink = cmsTransform2DeviceLink(xform, (cmsFloat64Number)4.3, (cmsUInt32Number)0);
    cmsSaveProfileToFile(devlink, "devlink.icc");

    // step 4: Cleanup
    (void)prevAdapt;
    (void)inFormat;
    (void)pcs;
    (void)adaptValue;
    (void)xform;
    (void)ctx;
    (void)devlink;
    // API sequence test completed successfully
    return 66;
}