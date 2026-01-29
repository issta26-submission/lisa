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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Prepare a white point and a Lab color, then convert to XYZ.
    cmsCIEXYZ whitePoint = { 0.9642, 1.0, 0.8249 };
    cmsCIELab lab = { 50.0, 20.0, -10.0 };
    cmsCIEXYZ xyz = { 0.0, 0.0, 0.0 };
    cmsLab2XYZ(&whitePoint, &xyz, &lab);

    // Create two simple (NULL) profiles for use in a multiprofile transform.
    cmsHPROFILE prof1 = cmsCreateNULLProfile();
    cmsHPROFILE prof2 = cmsCreateNULLProfile();

    // Set rendering intents on the profiles if created.
    if (prof1) cmsSetHeaderRenderingIntent(prof1, 0); // perceptual (example)
    if (prof2) cmsSetHeaderRenderingIntent(prof2, 1); // relative colorimetric (example)

    // Build an array of profiles and create a multiprofile transform.
    cmsHPROFILE profiles[2] = { prof1, prof2 };
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(nullptr, profiles, 2,
                                                               /*InputFormat*/ 0,
                                                               /*OutputFormat*/ 0,
                                                               /*Intent*/ 0,
                                                               /*dwFlags*/ 0);
    // Clean up the transform if created.
    if (transform) {
        cmsDeleteTransform(transform);
    }

    // Allocate and then free a dictionary handle to exercise cmsDictFree.
    cmsHANDLE dict = cmsDictAlloc(nullptr);
    if (dict) {
        cmsDictFree(dict);
    }

    return 66;
}