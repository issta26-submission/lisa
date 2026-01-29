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
//<ID> 137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Prepare contexts and containers
    cmsContext ctx = cmsDupContext(NULL, NULL);
    cmsUInt16Number alarmCodes[16];
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE devLink = NULL;
    cmsHTRANSFORM transform = NULL;
    cmsContext transformCtx = NULL;
    cmsUInt32Number version = 0;
    cmsUInt32Number creator = 0;

    // Retrieve default alarm codes and set them into the duplicated context
    cmsGetAlarmCodes(alarmCodes);
    cmsSetAlarmCodesTHR(ctx, alarmCodes);

    // Create an sRGB profile using the thread-aware API and query some header info
    hProfile = cmsCreate_sRGBProfileTHR(ctx);
    version = cmsGetEncodedICCversion(hProfile);
    creator = cmsGetHeaderCreator(hProfile);

    // Attempt to create a device link profile from a cube file (may return NULL if file missing)
    devLink = cmsCreateDeviceLinkFromCubeFile("example.cube");

    // Create a transform (input and output both the same profile here as a simple sequence)
    transform = cmsCreateTransformTHR(ctx, hProfile, 0U, hProfile, 0U, 0U, 0U);
    transformCtx = cmsGetTransformContextID(transform);

    // Unregister any plugins and delete the duplicated context
    cmsUnregisterPlugins();
    cmsDeleteContext(ctx);

    // Silence unused-variable warnings for the test harness
    (void)alarmCodes;
    (void)version;
    (void)creator;
    (void)devLink;
    (void)transform;
    (void)transformCtx;
    (void)hProfile;

    return 66;
}