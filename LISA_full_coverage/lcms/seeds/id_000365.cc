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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE prof = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE nullProf = cmsCreateNULLProfileTHR(ctx);
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsPipeline * lut = cmsPipelineAlloc(ctx, (cmsUInt32Number)3, (cmsUInt32Number)3);

    // step 2: Configuration
    cmsSetPCS(prof, (cmsColorSpaceSignature)0x58595A20); // 'XYZ ' as PCS
    cmsTagSignature descSig = (cmsTagSignature)0x64657363; // 'desc' tag signature
    cmsMLU * mlu = (cmsMLU *) cmsReadTag(prof, descSig);
    char descBuf[128];
    memset(descBuf, 0, sizeof(descBuf));
    cmsMLUgetUTF8(mlu, "en", "US", descBuf, (cmsUInt32Number)sizeof(descBuf));

    // step 3: Operate & Validate
    cmsFloat32Number in[3];
    cmsFloat32Number out[3];
    in[0] = (cmsFloat32Number) d50->x;
    in[1] = (cmsFloat32Number) d50->y;
    in[2] = (cmsFloat32Number) d50->Y;
    memset(out, 0, sizeof(out));
    cmsPipelineEvalFloat(in, out, lut);
    cmsContext ctx2 = cmsGetPipelineContextID(lut);
    (void)ctx2;
    (void)descBuf;
    (void)out;

    // step 4: Cleanup
    cmsPipelineUnlinkStage(lut, (cmsStageLoc)0, (cmsStage **)NULL);
    cmsUnregisterPlugins();
    cmsCloseProfile(prof);
    cmsCloseProfile(nullProf);

    // API sequence test completed successfully
    return 66;
}