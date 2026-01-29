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
    // Create a context for thread-safe THR variants
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) return 0;

    // Prepare a simple log handler (no-op)
    cmsLogErrorHandlerFunction logHandler =
        (cmsLogErrorHandlerFunction) [](cmsContext c, cmsUInt32Number code, const char * msg) {
            // avoid unused-parameter warnings
            (void)c; (void)code; (void)msg;
        };

    // Register the log handler for this context
    cmsSetLogErrorHandlerTHR(ctx, logHandler);

    // Allocate an IT8 handle (if available) and set a data cell (row/col)
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    cmsBool it8_ok = 0;
    if (it8) {
        // Try to set a sample value at row 0, col 0
        it8_ok = cmsIT8SetDataRowCol(it8, 0, 0, "SampleValue");
    }

    // Try to create a device link from a cube file (may fail if file not present)
    cmsHPROFILE devLink = cmsCreateDeviceLinkFromCubeFile("example.cube");

    // If device-link creation failed, fall back to creating a known profile
    cmsHPROFILE profile = devLink;
    if (!profile) {
        profile = cmsCreate_sRGBProfileTHR(ctx);
    }

    if (!profile) {
        if (it8) cmsIT8Free(it8);
        return 0;
    }

    // Query the header creation date/time
    struct tm created = {};
    cmsBool dt_ok = cmsGetHeaderCreationDateTime(profile, &created);

    // Clean up the IT8 handle if allocated
    if (it8) cmsIT8Free(it8);

    // Consider the sequence successful if we obtained a valid date/time
    if (dt_ok) return 66;
    return 0;
}