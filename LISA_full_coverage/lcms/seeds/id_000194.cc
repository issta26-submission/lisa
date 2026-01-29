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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE hInput = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE hOutput = cmsCreateNULLProfile();
    cmsHPROFILE hProof = cmsCreateNULLProfileTHR(ctx);
    cmsHTRANSFORM hTransform = cmsCreateProofingTransformTHR(ctx, hInput, (cmsUInt32Number)0, hOutput, (cmsUInt32Number)0, hProof, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsUInt32Number outFmt = cmsGetTransformOutputFormat(hTransform);
    cmsChangeBuffersFormat(hTransform, (cmsUInt32Number)0, outFmt);
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hInput);
    cmsCloseProfile(hOutput);
    cmsCloseProfile(hProof);
    (void)ctx;
    (void)outFmt;
    return 66;
}