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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Build a simple gamma tone curve
    cmsToneCurve * gamma = cmsBuildGamma((cmsContext) NULL, 2.2);

    // Step 2: Create some profiles using D50 white point helpers
    cmsHPROFILE hLab4 = cmsCreateLab4ProfileTHR((cmsContext) NULL, cmsD50_xyY());
    cmsHPROFILE hLab2 = cmsCreateLab2ProfileTHR((cmsContext) NULL, cmsD50_xyY());

    // Step 3: Tweak header and query some header info
    cmsSetHeaderRenderingIntent(hLab4, 0U);
    cmsUInt32Number creator = cmsGetHeaderCreator(hLab4);

    // Step 4: Create a transform between the two profiles
    cmsHTRANSFORM xform = cmsCreateTransformTHR((cmsContext) NULL,
                                               hLab4, 0U,
                                               hLab2, 0U,
                                               0U, 0U);

    // Step 5: Convert the transform into a device link and query named color list
    cmsHPROFILE devlink = cmsTransform2DeviceLink(xform, 4.0, 0U);
    cmsNAMEDCOLORLIST * ncl = cmsGetNamedColorList(xform);

    // Step 6: Check if a profile contains a CLUT and then clean up profiles
    cmsBool hasCLUT = cmsIsCLUT(hLab4, 0U, 0U);
    (void) gamma;
    (void) creator;
    (void) devlink;
    (void) ncl;
    (void) hasCLUT;

    cmsCloseProfile(hLab4);
    cmsCloseProfile(hLab2);

    return 66;
}