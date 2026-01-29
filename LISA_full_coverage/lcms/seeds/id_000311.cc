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
//<ID> 311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Create context and profiles
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE profile = cmsCreateNULLProfileTHR(ctx);
    cmsColorSpaceSignature cs = cmsGetColorSpace(profile);
    cmsUInt32Number channels = cmsChannelsOf(cs);
    cmsUInt32Number encVer = cmsGetEncodedICCversion(profile);
    cmsSetHeaderRenderingIntent(profile, encVer & 3u);
    cmsUInt32Number creator = cmsGetHeaderCreator(profile);
    cmsSetHeaderModel(profile, creator & 0xFFFFu);

    // Step 2: Additional profiles and stage creation
    cmsHPROFILE xyz = cmsCreateXYZProfileTHR(ctx);
    const cmsFloat64Number matrix[9] = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    };
    const cmsFloat64Number offset[3] = { 0.0, 0.0, 0.0 };
    cmsStage * stage = cmsStageAllocMatrix(ctx, 3, 3, matrix, offset);
    cmsStage * dup = cmsStageDup(stage);
    cmsStageSignature s1 = cmsStageType(stage);
    cmsStageSignature s2 = cmsStageType(dup);
    (void)s1;
    (void)s2;
    cmsStageFree(stage);
    cmsStageFree(dup);

    // Step 3: Query some profile info and save
    cmsFloat64Number ver = cmsGetProfileVersion(xyz);
    (void)ver;
    cmsSaveProfileToFile(profile, "out_profile.icc");
    cmsSetHeaderRenderingIntent(xyz, channels);

    // Step 4: Misc queries and duplicate context
    (void)cmsGetEncodedCMMversion();
    cmsContext ctx2 = cmsDupContext(ctx, NULL);
    (void)ctx2;

    /* API sequence test completed successfully */
    return 66;
}