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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Create a context and a Lab profile (using D50 white point)
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    const cmsCIExyY * wp = cmsD50_xyY();
    cmsHPROFILE hProfile = cmsCreateLab4Profile(wp);

    // Step 2: Configure profile header and version
    cmsUInt32Number model = 0x6D6F646CUL; /* 'modl' */
    cmsSetHeaderModel(hProfile, model);
    cmsSetProfileVersion(hProfile, 4.5);

    // Step 3: Query header rendering intent and profile's context, then duplicate context
    cmsUInt32Number renderingIntent = cmsGetHeaderRenderingIntent(hProfile);
    cmsContext profCtx = cmsGetProfileContextID(hProfile);
    cmsContext dupCtx = cmsDupContext(profCtx, NULL);

    // Step 4: Build a gamma tone curve on duplicated context, inspect and estimate it, reverse it
    cmsToneCurve * gamma = cmsBuildGamma(dupCtx, 2.2);
    cmsFloat64Number estimated = cmsEstimateGamma(gamma, 1e-6);
    cmsBool isDesc = cmsIsToneCurveDescending(gamma);
    cmsToneCurve * reversed = cmsReverseToneCurveEx(256, gamma);

    // Step 5: Open an IO handler from a memory buffer and save the profile to it, then close handler
    char membuf[512];
    cmsIOHANDLER * io = cmsOpenIOhandlerFromMem(dupCtx, membuf, sizeof(membuf), "w+");
    cmsUInt32Number bytesSaved = 0;
    if (io) {
        bytesSaved = cmsSaveProfileToIOhandler(hProfile, io);
        cmsCloseIOhandler(io);
    }

    // Silence unused variable warnings
    (void)ctx;
    (void)wp;
    (void)model;
    (void)renderingIntent;
    (void)profCtx;
    (void)dupCtx;
    (void)gamma;
    (void)estimated;
    (void)isDesc;
    (void)reversed;
    (void)membuf;
    (void)bytesSaved;
    (void)hProfile;

    return 66;
}