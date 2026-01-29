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
//<ID> 158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Prepare a sample xyY color
    cmsCIExyY src;
    src.x = 0.3127;   // approximate D65 x
    src.y = 0.3290;   // approximate D65 y
    src.Y = 50.0;     // luminance

    // Destination XYZ
    cmsCIEXYZ dest;
    cmsxyY2XYZ(&dest, &src);

    // Create a NULL profile
    cmsHPROFILE profile = cmsCreateNULLProfile();
    if (profile == NULL) {
        // creation failed
        return 1;
    }

    // Query whether a (likely absent) tag exists using a zero tag signature
    cmsBool hasTag = cmsIsTag(profile, (cmsTagSignature)0);

    // Attempt to duplicate an MLU from a null source (allowed syntactically)
    cmsMLU * duplicatedMLU = cmsMLUdup(nullptr);

    // Silence unused-variable warnings (we performed the calls above)
    (void)dest;
    (void)hasTag;
    (void)duplicatedMLU;

    return 66;
}