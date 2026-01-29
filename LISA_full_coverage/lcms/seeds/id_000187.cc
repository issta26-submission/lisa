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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE hProfile = cmsCreateNULLProfileTHR((cmsContext)NULL);
    cmsContext ctx = cmsGetProfileContextID(hProfile);
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    char * propNames = NULL;
    char ** propNamesArr = NULL;
    cmsUInt32Number propCount = 0;
    const char * propValue = NULL;
    char patchBuf[64];
    memset(patchBuf, 0, sizeof(patchBuf));
    const char * patchName = NULL;
    cmsUInt32Number model = 0x4D4F444CU; /* 'MODL' */
    cmsUInt32Number renderingIntent = 0;

    // step 2: Configure
    cmsSetHeaderModel(hProfile, model);
    cmsIT8SetPropertyUncooked(hIT8, "Manufacturer", "ACME Inc.");
    propCount = cmsIT8EnumProperties(hIT8, &propNamesArr);

    // step 3: Operate
    propValue = cmsIT8GetProperty(hIT8, "Manufacturer");
    renderingIntent = cmsGetHeaderRenderingIntent(hProfile);
    patchName = cmsIT8GetPatchName(hIT8, 0, patchBuf);

    // step 4: Validate & Cleanup
    cmsIT8Free(hIT8);
    (void)ctx;
    (void)propNames;
    (void)propNamesArr;
    (void)propCount;
    (void)propValue;
    (void)patchName;
    (void)patchBuf;
    (void)renderingIntent;
    (void)model;
    (void)hProfile;

    // API sequence test completed successfully
    return 66;
}