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
//<ID> 244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ctx);
    cmsUInt32Number creator_before = cmsGetHeaderCreator(hProfile);
    cmsUInt32Number flags_before = cmsGetHeaderFlags(hProfile);

    // step 2: Configure
    cmsSetHeaderModel(hProfile, (cmsUInt32Number)0x4D4F444CUL);
    cmsPipeline * lut = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);

    // step 3: Operate
    cmsBool on = (cmsBool)1;
    cmsBool off = (cmsBool)0;
    cmsPipelineSetSaveAs8bitsFlag(lut, on);
    cmsPipelineSetSaveAs8bitsFlag(lut, off);
    cmsPipeline * lut_dup = cmsPipelineDup(lut);

    // step 4: Validate & Cleanup
    cmsUInt32Number creator_after = cmsGetHeaderCreator(hProfile);
    cmsUInt32Number flags_after = cmsGetHeaderFlags(hProfile);
    cmsUInt32Number combined = creator_before ^ flags_before ^ creator_after ^ flags_after;
    (void)combined;
    (void)lut_dup;
    cmsCloseProfile(hProfile);
    // API sequence test completed successfully
    return 66;
}