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
//<ID> 267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR((cmsContext)NULL,
                                                          (cmsUInt32Number)16,
                                                          (cmsFloat64Number)50.0,
                                                          (cmsFloat64Number)1.1,
                                                          (cmsFloat64Number)0.0,
                                                          (cmsFloat64Number)1.0,
                                                          (cmsUInt32Number)6500,
                                                          (cmsUInt32Number)5000);
    cmsToneCurve * gamma = cmsBuildGamma((cmsContext)NULL, (cmsFloat64Number)2.2);
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList((cmsContext)NULL, (cmsUInt32Number)2, (cmsUInt32Number)3, "pre", "suf");
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)0;
    cmsUInt32Number namedCount = (cmsUInt32Number)0;
    const cmsCurveSegment * seg = (const cmsCurveSegment *)NULL;
    cmsUInt32Number model = (cmsUInt32Number)0x4D4F444CUL; /* 'MODL' */
    cmsColorSpaceSignature pcs = (cmsColorSpaceSignature)0x58595A20; /* 'XYZ ' */

    // step 2: Configure
    cmsSetHeaderModel(profile, model);
    cmsSetPCS(profile, pcs);

    // step 3: Operate
    seg = cmsGetToneCurveSegment((cmsInt32Number)0, (const cmsToneCurve *)gamma);
    namedCount = cmsNamedColorCount(ncl);
    cmsSaveProfileToMem(profile, (void *)NULL, &bytesNeeded);
    cmsUInt32Number combinedMetric = bytesNeeded + namedCount + (cmsUInt32Number)seg->Type;
    (void)combinedMetric;

    // step 4: Validate & Cleanup
    cmsCloseProfile(profile);
    cmsFreeToneCurve(gamma);
    cmsFreeNamedColorList(ncl);
    // API sequence test completed successfully
    return 66;
}