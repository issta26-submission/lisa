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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    unsigned char iccMemory[128] = { 0 };
    cmsHPROFILE profile = cmsOpenProfileFromMem(iccMemory, (cmsUInt32Number)sizeof(iccMemory));
    cmsHPROFILE srgb = cmsCreate_sRGBProfileTHR(ctx);
    cmsHTRANSFORM xform = cmsCreateTransformTHR(ctx, profile, (cmsUInt32Number)0, srgb, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsMLU * mlu = cmsMLUalloc(ctx, (cmsUInt32Number)1);
    cmsMLUsetASCII(mlu, "en", "US", "Test ASCII");
    char utf8buf[64] = { 0 };
    (void) cmsMLUgetUTF8(mlu, "en", "US", utf8buf, (cmsUInt32Number)sizeof(utf8buf));
    cmsMLUfree(mlu);
    int ver = cmsGetEncodedCMMversion();
    (void) ver;
    (void) xform;
    (void) profile;
    (void) srgb;
    (void) ctx;
    return 66;
}