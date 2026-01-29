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
//<ID> 268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsUInt32Number model = (cmsUInt32Number)0x4D4F444CUL; /* 'MODL' */
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ctx, (cmsUInt32Number)17, (cmsFloat64Number)0.5, (cmsFloat64Number)1.1, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)6500, (cmsUInt32Number)5000);
    cmsToneCurve * curve = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    const cmsCurveSegment * seg = cmsGetToneCurveSegment((cmsInt32Number)0, curve);
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList(ctx, (cmsUInt32Number)5, (cmsUInt32Number)3, "Pre", "Suf");
    cmsUInt32Number ncount = cmsNamedColorCount(ncl);

    // step 2: Configure
    cmsSetHeaderModel(profile, model);
    cmsSetPCS(profile, (cmsColorSpaceSignature)0x4C616220); /* 'Lab ' */
    cmsUInt32Number flags = cmsGetHeaderFlags(profile);

    // step 3: Operate
    cmsUInt32Number needed = (cmsUInt32Number)0;
    cmsBool ok = cmsSaveProfileToMem(profile, NULL, &needed);
    cmsUInt32Number allocSize = (needed < 4U) ? 4U : needed;
    void * mem = malloc((size_t) allocSize);
    cmsSaveProfileToMem(profile, mem, &needed);
    unsigned char * bytes = (unsigned char *) mem;
    bytes[0] = (unsigned char)(ncount & 0xFFU);
    bytes[1] = (unsigned char)((seg != NULL) ? (seg->Type & 0xFF) : 0U);
    bytes[2] = (unsigned char)(flags & 0xFFU);
    bytes[3] = (unsigned char)(ok ? 1 : 0);

    // step 4: Validate & Cleanup
    cmsFreeToneCurve(curve);
    cmsFreeNamedColorList(ncl);
    cmsCloseProfile(profile);
    free(mem);
    (void)ctx;
    // API sequence test completed successfully
    return 66;
}