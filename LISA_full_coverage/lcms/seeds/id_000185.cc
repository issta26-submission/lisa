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
//<ID> 185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsHPROFILE hProfile = NULL;
    cmsHANDLE hIT8 = NULL;
    cmsContext ctx = NULL;
    char ** propNames = NULL;
    cmsUInt32Number propCount = 0;
    char patchBuf[128];
    const char * patchName = NULL;
    const char * propValue = NULL;
    cmsUInt32Number renderingIntent = 0;
    cmsUInt32Number modelValue = 0x4D4F444CU; /* 'MODL' */

    // step 2: Setup
    hProfile = cmsCreateLab4ProfileTHR(NULL, NULL);
    ctx = cmsDupContext(NULL, NULL);
    hIT8 = cmsDictAlloc(ctx);

    // step 3: Core operations
    // Retrieve some textual info from the profile into patchBuf (use Info type 0 as a generic request)
    propCount = cmsGetProfileInfoUTF8(hProfile, (cmsInfoType)0, "eng", "USA", patchBuf, sizeof(patchBuf));
    propValue = patchBuf;
    patchName = propValue;
    renderingIntent = (cmsUInt32Number)cmsGetProfileVersion(hProfile);
    cmsSetHeaderAttributes(hProfile, (cmsUInt64Number) modelValue);

    // step 4: Validate & Cleanup
    cmsCloseProfile(hProfile);
    (void)propCount;
    (void)propNames;
    (void)propValue;
    (void)patchName;
    (void)patchBuf;
    (void)renderingIntent;
    (void)modelValue;
    (void)hIT8;
    (void)ctx;

    // API sequence test completed successfully
    return 66;
}