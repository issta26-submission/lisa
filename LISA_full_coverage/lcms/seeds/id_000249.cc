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
//<ID> 249
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
    cmsUInt32Number model = (cmsUInt32Number)0x4D4F444CUL; /* 'MODL' */
    cmsPipeline * lut = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);
    char info[8];
    memset(info, 0, sizeof(info));

    // step 2: Configure
    cmsSetHeaderModel(profile, model);
    cmsPipelineSetSaveAs8bitsFlag(lut, (cmsBool)1);

    // step 3: Operate
    cmsUInt32Number creator = cmsGetHeaderCreator(profile);
    cmsUInt32Number flags = cmsGetHeaderFlags(profile);
    cmsPipelineSetSaveAs8bitsFlag(lut, (cmsBool)(flags & 1U));
    info[0] = (char)(creator & 0xFFU);
    info[1] = (char)(flags & 0xFFU);

    // step 4: Validate & Cleanup
    (void)ctx;
    (void)lut;
    (void)info;
    cmsCloseProfile(profile);

    // API sequence test completed successfully
    return 66;
}