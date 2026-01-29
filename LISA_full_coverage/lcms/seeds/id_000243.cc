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
//<ID> 243
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

    // step 2: Configure
    cmsSetHeaderModel(hProfile, (cmsUInt32Number)0x4D4F444CUL); // 'MODL'
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);

    // step 3: Operate
    cmsPipeline * lut = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);
    cmsBool flagOn = (cmsBool)((creator | flags) & 1u);
    cmsBool set1 = cmsPipelineSetSaveAs8bitsFlag(lut, flagOn);
    cmsPipeline * dup = cmsPipelineDup(lut);
    cmsBool set2 = cmsPipelineSetSaveAs8bitsFlag(dup, (cmsBool)(!flagOn));

    // step 4: Validate & Cleanup
    (void)ctx;
    (void)creator;
    (void)flags;
    (void)set1;
    (void)set2;
    (void)lut;
    (void)dup;
    cmsCloseProfile(hProfile);

    // API sequence test completed successfully
    return 66;
}