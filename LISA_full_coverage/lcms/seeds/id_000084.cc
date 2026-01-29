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
//<ID> 84
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsHPROFILE hProfile = cmsCreateNULLProfile();
    cmsToneCurve * tone = NULL;
    cmsToneCurve * invTone = NULL;
    cmsStage * stage = NULL;
    cmsContext ctx = cmsGetProfileContextID(hProfile);
    cmsUInt32Number ver = 0;
    cmsUInt32Number tagCount = 0;

    // step 2: Setup
    tone = cmsBuildGamma(ctx, 2.2);
    invTone = cmsReverseToneCurve(tone);
    stage = cmsStageAllocToneCurves(ctx, 1, (cmsToneCurve * const *)&tone);

    // step 3: Core operations
    ver = cmsGetEncodedICCversion(hProfile);
    cmsSetEncodedICCversion(hProfile, ver + 0x00010000U);
    cmsSetHeaderModel(hProfile, 0x4D4F444CU); // arbitrary model code 'MODL'
    tagCount = (cmsUInt32Number) cmsGetTagCount(hProfile);
    cmsStageFree(stage);

    // step 4: Cleanup
    cmsFreeToneCurve(invTone);
    cmsFreeToneCurve(tone);

    (void)tagCount;
    (void)ctx;
    (void)ver;

    return 66; // API sequence test completed successfully
}