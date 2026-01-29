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
//<ID> 263
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    char prefix[8];
    char suffix[8];
    memset(prefix, 0, sizeof(prefix));
    memset(suffix, 0, sizeof(suffix));
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ctx, (cmsUInt32Number)5, (cmsFloat64Number)50.0, (cmsFloat64Number)10.0, (cmsFloat64Number)15.0, (cmsFloat64Number)1.1, (cmsUInt32Number)6500, (cmsUInt32Number)5000);
    cmsUInt32Number model = (cmsUInt32Number)0x4D4F444CUL; /* 'MODL' */
    cmsColorSpaceSignature pcs = (cmsColorSpaceSignature)0x4C616220; /* 'Lab ' */
    cmsToneCurve * tone = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsNAMEDCOLORLIST * nlist = cmsAllocNamedColorList(ctx, (cmsUInt32Number)2, (cmsUInt32Number)3, prefix, suffix);

    // step 2: Configure
    cmsSetHeaderModel(profile, model);
    cmsSetPCS(profile, pcs);

    // step 3: Operate
    const cmsCurveSegment * seg = cmsGetToneCurveSegment((cmsInt32Number)0, tone);
    cmsUInt32Number named_count = cmsNamedColorCount(nlist);
    cmsUInt32Number seg_type = seg ? (cmsUInt32Number) seg->Type : (cmsUInt32Number)0;
    cmsUInt32Number seg_grid = seg ? seg->nGridPoints : (cmsUInt32Number)0;
    (void)seg;
    (void)seg_type;
    (void)seg_grid;

    // step 4: Validate & Cleanup
    (void)named_count;
    cmsFreeToneCurve(tone);
    cmsFreeNamedColorList(nlist);
    cmsCloseProfile(profile);
    // API sequence test completed successfully
    return 66;
}