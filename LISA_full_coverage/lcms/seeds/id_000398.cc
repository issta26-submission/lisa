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
//<ID> 398
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Create contexts
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsContext dupCtx = cmsDupContext(ctx, NULL);

    // Step 2: Prepare a D50 white point and tweak it
    const cmsCIExyY * d50 = cmsD50_xyY();
    cmsCIExyY wp;
    memcpy(&wp, d50, sizeof(wp));
    wp.x = 0.34567;
    wp.y = 0.35850;
    wp.Y = 1.0;

    // Step 3: Create profiles using the context
    cmsHPROFILE hLab = cmsCreateLab2ProfileTHR(ctx, &wp);
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();

    // Step 4: Query some simple profile/context information
    cmsContext profCtx = cmsGetProfileContextID(hLab);
    cmsUInt32Number tagCount = cmsGetTagCount(hLab);
    cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hLab);
    cmsUInt32Number model = cmsGetHeaderModel(hLab);

    // Step 5: Do a trivial deterministic computation based on gathered data
    unsigned int validation = (unsigned int)((unsigned int)(wp.x * 1000.0) ^ tagCount);
    validation += (unsigned int)manufacturer;
    validation += (unsigned int)model;
    validation += (profCtx ? 1u : 0u);
    (void) validation; /* keep value used so optimizer won't drop code */

    // Step 6: Allocate and free some auxiliary objects
    cmsHANDLE dict = cmsDictAlloc(ctx);
    cmsDictFree(dict);

    cmsMLU * mlu = cmsMLUalloc(ctx, 1);
    cmsMLUsetASCII(mlu, "eng", "USA", "Test string");
    (void) cmsMLUtranslationsCount(mlu);

    // Step 7: Misc cleanup
    cmsUnregisterPlugins();
    cmsDeleteContext(dupCtx);
    cmsDeleteContext(ctx);

    // Note: some profile objects (hLab, hSRGB) are not explicitly closed here
    // because the minimal exported API list provided does not include a close function.
    return 66;
}