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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsPipeline *lut = NULL;
    cmsHANDLE hDict = NULL;
    cmsHANDLE hDictDupRes = NULL;
    cmsUInt8Number profileID[16];
    cmsUInt32Number inChan = 3U;
    cmsUInt32Number outChan = 3U;
    cmsUInt32Number model = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number encver = 0;

    // Step 2: Setup
    hProfile = cmsCreateProfilePlaceholder(ctx);
    lut = cmsPipelineAlloc(ctx, inChan, outChan);
    memset(profileID, 0, sizeof(profileID));
    hDict = cmsDictAlloc(ctx);

    // Step 3: Operate -> Validate
    hDictDupRes = cmsDictDup(hDict);
    cmsSetProfileVersion(hProfile, 4.3);
    cmsSetEncodedICCversion(hProfile, 0x0420);
    model = cmsGetHeaderModel(hProfile);
    intent = cmsGetHeaderRenderingIntent(hProfile);
    encver = cmsGetEncodedICCversion(hProfile);
    cmsPipelineSetSaveAs8bitsFlag(lut, 1);

    // Step 4: Cleanup
    cmsPipelineFree(lut);
    (void)ctx;
    (void)hDict;
    (void)hDictDupRes;
    (void)model;
    (void)intent;
    (void)encver;
    (void)profileID;
    (void)inChan;
    (void)outChan;
    // API sequence test completed successfully
    return 66;
}