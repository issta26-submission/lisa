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
    // Create a context for library operations
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) return 1;

    // Allocate a GBD (Grid Builder Data) handle using the context
    cmsHANDLE gbd = cmsGBDAlloc(ctx);
    if (!gbd) {
        cmsDeleteContext(ctx);
        return 2;
    }

    // Create an XYZ profile
    cmsHPROFILE hProfile = cmsCreateXYZProfile();
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 3;
    }

    // Prepare a floating-point CIEXYZ and encode to 16-bit representation
    cmsCIEXYZ fXYZ;
    fXYZ.X = 0.95047; // D65 X
    fXYZ.Y = 1.00000; // D65 Y
    fXYZ.Z = 1.08883; // D65 Z

    cmsUInt16Number encodedXYZ[3] = {0, 0, 0};
    cmsFloat2XYZEncoded(encodedXYZ, &fXYZ);

    // Close the profile
    cmsBool closed = cmsCloseProfile(hProfile);
    if (!closed) {
        cmsDeleteContext(ctx);
        return 4;
    }

    // Clean up context (no public API provided here to free gbd; keep minimal and safe)
    cmsDeleteContext(ctx);

    return 66;
}