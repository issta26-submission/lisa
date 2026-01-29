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
//<ID> 399
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Declarations
    cmsContext ctx = NULL;
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    cmsHPROFILE labProfile = cmsCreateLab2Profile(&whitePoint);

    // Query header and profile information
    cmsUInt32Number headerModel = cmsGetHeaderModel(labProfile);
    cmsUInt32Number headerManufacturer = cmsGetHeaderManufacturer(labProfile);
    cmsUInt32Number headerCreator = cmsGetHeaderCreator(labProfile);
    cmsUInt32Number headerRenderingIntent = cmsGetHeaderRenderingIntent(labProfile);
    cmsFloat64Number version = cmsGetProfileVersion(labProfile);
    cmsUInt32Number colorSpace = (cmsUInt32Number) cmsGetColorSpace(labProfile);
    cmsUInt32Number pcs = (cmsUInt32Number) cmsGetPCS(labProfile);

    // Attempt to get serialized size for the profile
    cmsUInt32Number bytesNeeded = 0;
    cmsUInt32Number saved = (cmsUInt32Number) cmsSaveProfileToMem(labProfile, NULL, &bytesNeeded);

    // Compute MD5 id for the profile
    cmsUInt32Number md5ok = (cmsUInt32Number) cmsMD5computeID(labProfile);

    // Keep a volatile aggregate so values are "used" (prevents optimizing them away)
    volatile cmsUInt32Number validation_sum = headerModel + headerManufacturer + headerCreator + headerRenderingIntent
        + (cmsUInt32Number) version + colorSpace + pcs + bytesNeeded + saved + md5ok;
    (void)validation_sum;

    // Unregister any thread-specific plugins for the (NULL) context
    cmsUnregisterPluginsTHR(ctx);

    // Return the expected sentinel value
    return 66;
}