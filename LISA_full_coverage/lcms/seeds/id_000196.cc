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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Create a context and profiles
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE hXYZ = cmsCreateXYZProfileTHR(ctx);
    cmsHPROFILE hNull = cmsCreateNULLProfileTHR(ctx);
    cmsHPROFILE hOkLab = cmsCreate_OkLabProfile(ctx);

    // step 2: Create a transform between profiles
    cmsHTRANSFORM hTransform = cmsCreateTransform(hXYZ, (cmsUInt32Number)0, hNull, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 3: Query transform/context/profile properties
    cmsContext txCtx = cmsGetTransformContextID(hTransform);
    cmsUInt32Number outFormat = cmsGetTransformOutputFormat(hTransform);
    cmsFloat64Number tac = cmsDetectTAC(hXYZ);
    cmsFloat64Number gamma = cmsDetectRGBProfileGamma(hXYZ, (cmsFloat64Number)2.2);
    cmsUInt32Number cs_code = _cmsLCMScolorSpace(cmsGetColorSpace(hXYZ));

    // step 4: Modify a profile header value
    cmsSetProfileVersion(hOkLab, (cmsFloat64Number)4.50);

    // step 5: Cleanup transform (profiles intentionally left as-is because no close API is in the provided set)
    cmsDeleteTransform(hTransform);

    (void)ctx;
    (void)hXYZ;
    (void)hNull;
    (void)hOkLab;
    (void)txCtx;
    (void)outFormat;
    (void)tac;
    (void)gamma;
    (void)cs_code;

    return 66;
}