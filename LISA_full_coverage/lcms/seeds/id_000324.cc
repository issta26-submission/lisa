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
//<ID> 324
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = NULL;
    cmsHPROFILE srgb = cmsCreate_sRGBProfile();
    cmsHPROFILE nullProfile = cmsCreateNULLProfileTHR(ctx);

    cmsHTRANSFORM xform = cmsCreateProofingTransform(
        srgb,
        (cmsUInt32Number)0,
        nullProfile,
        (cmsUInt32Number)0,
        srgb,
        (cmsUInt32Number)0,
        (cmsUInt32Number)0,
        (cmsUInt32Number)0
    );

    cmsContext tctx = cmsGetTransformContextID(xform);
    cmsHPROFILE nullProfile2 = cmsCreateNULLProfileTHR(tctx);

    cmsUInt32Number enc = cmsGetEncodedICCversion(nullProfile);
    cmsSetEncodedICCversion(nullProfile2, enc);

    (void)enc;
    (void)tctx;

    cmsDeleteTransform(xform);
    cmsUnregisterPlugins();

    return 66;
}