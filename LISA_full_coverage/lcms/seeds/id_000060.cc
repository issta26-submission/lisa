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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Declarations & Initialize core context and IO handler
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsIOHANDLER * io = cmsOpenIOhandlerFromNULL(ctx);
    const cmsCIEXYZ * d50_xyz = cmsD50_XYZ();

    // Step 2: Configure whitepoint and create a Lab profile using the context
    cmsCIExyY d50_xyY;
    d50_xyY.x = 0.34567;
    d50_xyY.y = 0.35850;
    d50_xyY.Y = 1.0;
    cmsHPROFILE hProfile = cmsCreateLab2ProfileTHR(ctx, &d50_xyY);

    // Step 3: Operate and validate profile contents
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    cmsCIELab lab;
    cmsXYZ2Lab(d50_xyz, &lab, d50_xyz);
    cmsContext profileCtx = cmsGetProfileContextID(hProfile);

    // Step 4: Cleanup resources
    cmsCloseProfile(hProfile);
    cmsCloseIOhandler(io);
    cmsDeleteContext(ctx);

    (void) d50_xyz;
    (void) lab;
    (void) tagCount;
    (void) profileCtx;
    (void) io;

    // API sequence test completed successfully
    return 66;
}