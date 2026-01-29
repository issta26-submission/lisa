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
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsCIExyY whitePoint;
    memset(&whitePoint, 0, sizeof(whitePoint));
    cmsHPROFILE hProfile = cmsCreateLab4Profile(&whitePoint);
    cmsContext ctx = cmsGetProfileContextID(hProfile);
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    char ** propNames = NULL;
    char patchBuf[128];
    memset(patchBuf, 0, sizeof(patchBuf));
    const char * propValue = NULL;
    const char * patchName = NULL;
    cmsUInt32Number propCount = 0;
    cmsUInt32Number initialModel = 0x4D4F444CU; /* 'MODL' */

    // step 2: Setup (configure IT8 properties and initial profile model)
    cmsIT8SetPropertyUncooked(hIT8, "Device", "ACME Inc.");
    cmsSetHeaderModel(hProfile, initialModel);

    // step 3: Operate (query rendering intent, update model, enumerate and fetch properties/patch)
    cmsUInt32Number renderingIntent = cmsGetHeaderRenderingIntent(hProfile);
    cmsSetHeaderModel(hProfile, renderingIntent);
    propCount = cmsIT8EnumProperties(hIT8, &propNames);
    propValue = cmsIT8GetProperty(hIT8, "Device");
    patchName = cmsIT8GetPatchName(hIT8, 0, patchBuf);

    // step 4: Cleanup & validate
    cmsIT8Free(hIT8);
    (void)whitePoint;
    (void)ctx;
    (void)propNames;
    (void)propCount;
    (void)propValue;
    (void)patchName;
    (void)patchBuf;
    (void)initialModel;
    (void)renderingIntent;
    (void)hProfile;

    // API sequence test completed successfully
    return 66;
}