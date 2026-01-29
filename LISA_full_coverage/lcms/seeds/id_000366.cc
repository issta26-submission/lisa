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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a context
    cmsContext ctx = cmsCreateContext(NULL, NULL);

    // Build a white point from a typical D65-ish temperature (6504K)
    cmsCIExyY wp;
    cmsWhitePointFromTemp(&wp, 6504.0);

    // Build a simple parametric tone curve (type 1, single-parameter gamma)
    cmsFloat64Number params[1] = { 2.2 };
    cmsToneCurve * curve = cmsBuildParametricToneCurve(ctx, 1, params);

    // Create a gray profile using the white point and transfer function
    cmsHPROFILE gray = cmsCreateGrayProfileTHR(ctx, &wp, curve);

    // Set header flags to 0 (no special flags)
    cmsSetHeaderFlags(gray, (cmsUInt32Number)0);

    // Compute profile ID (MD5) for the profile
    cmsBool md5ok = cmsMD5computeID(gray);

    // Open a NULL IO handler and save the profile to it
    cmsIOHANDLER * io = cmsOpenIOhandlerFromNULL(ctx);
    cmsUInt32Number saved = cmsSaveProfileToIOhandler(gray, io);

    // Close IO handler
    cmsCloseIOhandler(io);

    // Unregister any plugins and delete the created context
    cmsUnregisterPlugins();
    cmsDeleteContext(ctx);

    // Silence unused-variable warnings
    (void)md5ok;
    (void)saved;
    (void)curve;
    (void)gray;
    (void)wp;

    return 66;
}