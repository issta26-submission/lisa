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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Context creation
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    const cmsCIExyY * d50 = cmsD50_xyY();
    char utf8buf[64];
    memset(utf8buf, 0, sizeof utf8buf);
    cmsMLU * mlu = (cmsMLU *) cmsReadTag(profile, (cmsTagSignature)0x64657363); // 'desc'
    cmsUInt32Number requested = (cmsUInt32Number)1;

    // step 2: Configure profile and compute a buffer size based on D50 whitepoint
    cmsSetPCS(profile, (cmsColorSpaceSignature)0x4C616200); // 'Lab\0'
    requested = (cmsUInt32Number)(((unsigned)((d50->x + d50->y + d50->Y) * 100.0)) % (sizeof utf8buf - 1u)) + 1u;

    // step 3: Operate - try to extract UTF8 from the profile description MLU into buffer
    cmsUInt32Number bytesWritten = cmsMLUgetUTF8((const cmsMLU *)mlu, "en", "US", utf8buf, requested);
    (void)bytesWritten;
    (void)utf8buf;
    (void)requested;

    // step 4: Cleanup
    cmsUnregisterPlugins();
    // API sequence test completed successfully
    return 66;
}