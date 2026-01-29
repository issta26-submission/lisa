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
//<ID> 265
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    const cmsCurveSegment * seg = cmsGetToneCurveSegment((cmsInt32Number)0, gamma);
    cmsNAMEDCOLORLIST * nlist = cmsAllocNamedColorList(ctx, (cmsUInt32Number)3, (cmsUInt32Number)4, "PFX", "SFX");
    cmsUInt32Number ncount = cmsNamedColorCount(nlist);
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ctx, (cmsUInt32Number)17, (cmsFloat64Number)0.5, (cmsFloat64Number)0.1, (cmsFloat64Number)0.0, (cmsFloat64Number)1.0, (cmsUInt32Number)5000, (cmsUInt32Number)6500);
    cmsUInt32Number model = (cmsUInt32Number)0x4D4F444CUL; /* 'MODL' */
    cmsColorSpaceSignature pcs = (cmsColorSpaceSignature)0x58595A20; /* 'XYZ ' */
    char mem[8192];
    memset(mem, 0, sizeof(mem));
    cmsUInt32Number bytes = (cmsUInt32Number)sizeof(mem);

    // step 2: Configure
    cmsSetHeaderModel(profile, model);
    cmsSetPCS(profile, pcs);
    (void)cmsSaveProfileToMem(profile, mem, &bytes);

    // step 3: Operate
    cmsHPROFILE profile2 = cmsOpenProfileFromMem(mem, bytes);
    cmsSetPCS(profile2, pcs);
    cmsUInt32Number segTypeAsModel = (cmsUInt32Number)(seg->Type);
    cmsSetHeaderModel(profile2, segTypeAsModel);
    (void)ncount;

    // step 4: Validate & Cleanup
    (void)ctx;
    (void)gamma;
    (void)seg;
    (void)nlist;
    (void)model;
    (void)pcs;
    (void)mem;
    (void)bytes;
    cmsCloseProfile(profile);
    cmsCloseProfile(profile2);
    // API sequence test completed successfully
    return 66;
}