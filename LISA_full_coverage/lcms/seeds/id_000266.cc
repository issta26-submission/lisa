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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsUInt32Number nLUTPoints = (cmsUInt32Number)17;
    cmsFloat64Number Bright = (cmsFloat64Number)1.0;
    cmsFloat64Number Contrast = (cmsFloat64Number)1.0;
    cmsFloat64Number Hue = (cmsFloat64Number)0.0;
    cmsFloat64Number Saturation = (cmsFloat64Number)1.0;
    cmsUInt32Number TempSrc = (cmsUInt32Number)6500;
    cmsUInt32Number TempDest = (cmsUInt32Number)5000;
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);

    // step 2: Configure
    cmsUInt32Number model = (cmsUInt32Number)0x4D4F444CUL; /* 'MODL' */
    cmsSetHeaderModel(profile, model);
    cmsColorSpaceSignature pcs = (cmsColorSpaceSignature)0x58595A20; /* 'XYZ ' */
    cmsSetPCS(profile, pcs);

    // step 3: Operate
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    const cmsCurveSegment * seg = cmsGetToneCurveSegment((cmsInt32Number)0, gamma);
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3, "pre", "suf");
    cmsUInt32Number namedCount = cmsNamedColorCount(ncl);
    unsigned char profileMem[4096];
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)sizeof(profileMem);
    cmsBool saved = cmsSaveProfileToMem(profile, (void *)profileMem, &bytesNeeded);

    // step 4: Validate & Cleanup
    char info[8];
    memset(info, 0, sizeof(info));
    info[0] = (char)((seg && ((cmsInt32Number)seg->Type >= 0)) ? (seg->Type & 0xFF) : 0);
    info[1] = (char)(namedCount & 0xFFU);
    (void)saved;
    (void)bytesNeeded;
    (void)info;
    if (profile) cmsCloseProfile(profile);
    if (gamma) cmsFreeToneCurve(gamma);
    if (ncl) cmsFreeNamedColorList(ncl);
    // API sequence test completed successfully
    return 66;
}