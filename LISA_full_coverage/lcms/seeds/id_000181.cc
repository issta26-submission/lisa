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
//<ID> 181
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
    char ** PropertyNames = NULL;
    cmsUInt32Number propCount = 0;
    char patchBuffer[128];
    char propBufferPlaceholder[128];

    // step 2: Setup (enumerate properties and query one property & patch name)
    propCount = cmsIT8EnumProperties(hIT8, &PropertyNames);
    const char * firstProperty = PropertyNames ? PropertyNames[0] : NULL;
    const char * propValue = cmsIT8GetProperty(hIT8, firstProperty);
    const char * patchName = cmsIT8GetPatchName(hIT8, 0, patchBuffer);

    // step 3: Operate (set and query profile header fields)
    cmsSetHeaderModel(hProfile, 0x74657374U); /* 'test' */
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);

    // step 4: Cleanup & Validation
    cmsIT8Free(hIT8);

    (void)whitePoint;
    (void)PropertyNames;
    (void)propCount;
    (void)propValue;
    (void)patchName;
    (void)propBufferPlaceholder;
    (void)patchBuffer;
    (void)intent;
    (void)hProfile;

    // API sequence test completed successfully
    return 66;
}