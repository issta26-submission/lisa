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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a context (plugin and user data NULL for a simple context)
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);

    // Prepare a user-data object for duplication (address is fine; not dereferenced here)
    static int user_data = 42;
    cmsContext dupCtx = nullptr;
    if (ctx) {
        // Duplicate the context with new user data
        dupCtx = cmsDupContext(ctx, &user_data);
    }

    // Create an sRGB profile
    cmsHPROFILE sRGB = cmsCreate_sRGBProfile();

    // Change adaptation state, storing previous value to restore later
    cmsFloat64Number prevAdapt = cmsSetAdaptationState(1.0); // set to 1.0 (D65-like) as an example

    // Build a Lab value and encode it to the v2 16-bit encoded representation
    cmsCIELab lab;
    lab.L = 50.0;
    lab.a = 10.0;
    lab.b = -20.0;

    cmsUInt16Number encodedLab[3] = {0, 0, 0};
    cmsFloat2LabEncodedV2(encodedLab, &lab);

    // Restore previous adaptation state
    (void) cmsSetAdaptationState(prevAdapt);

    // Avoid unused variable warnings; no explicit close/free APIs used here in the provided list
    (void) ctx;
    (void) dupCtx;
    (void) sRGB;
    (void) encodedLab;

    return 66;
}