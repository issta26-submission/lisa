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
//<ID> 246
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
    cmsUInt32Number modelSig = (cmsUInt32Number)0x4D4F444CUL; /* 'MODL' */
    cmsSetHeaderModel(hProfile, modelSig);
    cmsPipeline * lut = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);

    // step 2: Configure
    cmsBool saveAs8_initial = (cmsBool)1;
    cmsPipelineSetSaveAs8bitsFlag(lut, saveAs8_initial);

    // step 3: Operate
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    cmsUInt32Number flags = cmsGetHeaderFlags(hProfile);
    cmsBool saveAs8_derived = (cmsBool)((creator ^ flags) & 1U);
    cmsPipelineSetSaveAs8bitsFlag(lut, saveAs8_derived);

    // step 4: Validate & Cleanup
    cmsCloseProfile(hProfile);
    (void)ctx;
    (void)lut;
    (void)modelSig;
    (void)creator;
    (void)flags;
    (void)saveAs8_initial;
    (void)saveAs8_derived;

    // API sequence test completed successfully
    return 66;
}