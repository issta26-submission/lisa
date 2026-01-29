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
//<ID> 264
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsUInt32Number model = (cmsUInt32Number)0x4D4F444CUL; /* 'MODL' */
    cmsColorSpaceSignature pcs = (cmsColorSpaceSignature)0x4C616220; /* 'Lab ' */
    cmsHPROFILE hProfile = cmsCreateBCHSWabstractProfileTHR(ctx, (cmsUInt32Number)16, (cmsFloat64Number)50.0, (cmsFloat64Number)1.2, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)6500, (cmsUInt32Number)5000);
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList(ctx, (cmsUInt32Number)8, (cmsUInt32Number)3, "PFX", "SFX");
    cmsToneCurve * curve = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);

    // step 2: Configure
    cmsSetHeaderModel(hProfile, model);
    cmsSetPCS(hProfile, pcs);

    // step 3: Operate
    cmsUInt32Number named_count = cmsNamedColorCount(ncl);
    const cmsCurveSegment * seg0 = cmsGetToneCurveSegment((cmsInt32Number)0, curve);
    (void)named_count;
    (void)seg0;

    // step 4: Validate & Cleanup
    cmsFreeToneCurve(curve);
    cmsFreeNamedColorList(ncl);
    cmsCloseProfile(hProfile);
    // API sequence test completed successfully
    return 66;
}