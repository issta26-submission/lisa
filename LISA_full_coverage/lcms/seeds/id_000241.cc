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
//<ID> 241
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE profile = cmsCreateProfilePlaceholder(ctx);
    cmsUInt32Number model = (cmsUInt32Number)0x4D4F444CUL;

    // step 2: Configure
    cmsSetHeaderModel(profile, model);
    cmsColorSpaceSignature pcs = cmsGetPCS(profile);
    cmsUInt32Number channels = cmsChannelsOf(pcs);
    cmsPipeline * lut = cmsPipelineAlloc(ctx, channels, channels);
    cmsPipelineSetSaveAs8bitsFlag(lut, (cmsBool)1);
    cmsPipeline * dup = cmsPipelineDup(lut);

    // step 3: Operate
    cmsUInt32Number creator = cmsGetHeaderCreator(profile);
    cmsUInt32Number flags = cmsGetHeaderFlags(profile);

    // step 4: Validate & Cleanup
    cmsCloseProfile(profile);
    (void)ctx;
    (void)model;
    (void)pcs;
    (void)channels;
    (void)lut;
    (void)dup;
    (void)creator;
    (void)flags;
    // API sequence test completed successfully
    return 66;
}