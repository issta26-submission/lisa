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
//<ID> 242
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
    cmsUInt32Number initialModel = (cmsUInt32Number)0x52474220U; // 'RGB '
    cmsSetHeaderModel(hProfile, initialModel);

    // step 2: Configure
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
    cmsPipeline * lut = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);

    // step 3: Operate
    cmsBool set8 = cmsPipelineSetSaveAs8bitsFlag(lut, (cmsBool)1);
    cmsUInt32Number newModel = creator ^ flags;
    cmsSetHeaderModel(hProfile, newModel);

    // step 4: Validate & Cleanup
    cmsUInt32Number finalCreator = cmsGetHeaderCreator(hProfile);
    cmsUInt32Number finalFlags = cmsGetHeaderFlags(hProfile);
    cmsCloseProfile(hProfile);
    (void)ctx;
    (void)initialModel;
    (void)creator;
    (void)flags;
    (void)lut;
    (void)set8;
    (void)newModel;
    (void)finalCreator;
    (void)finalFlags;
    // API sequence test completed successfully
    return 66;
}