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
//<ID> 369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    cmsUInt32Number pcs_sig = (cmsUInt32Number)((cmsUInt32Number)(d50->x * 1000.0));
    char utf8buf[128];
    memset(utf8buf, 0, sizeof(utf8buf));

    // step 2: Setup
    cmsSetPCS(profile, (cmsColorSpaceSignature)pcs_sig);
    cmsUInt32Number len = cmsMLUgetUTF8(NULL, "eng", "USA", utf8buf, (cmsUInt32Number)sizeof(utf8buf));

    // step 3: Operate & Validate
    cmsUInt32Number validation = (cmsUInt32Number)(pcs_sig ^ len);
    (void)validation;

    // step 4: Cleanup
    cmsUnregisterPlugins();

    // API sequence test completed successfully
    return 66;
}