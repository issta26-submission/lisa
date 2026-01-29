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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = (cmsContext)NULL;

    static const float values[2] = { 0.0f, 1.0f };
    cmsToneCurve * curves[3];
    curves[0] = cmsBuildTabulatedToneCurveFloat(ctx, (cmsUInt32Number)2, (const cmsFloat32Number*)values);
    curves[1] = cmsBuildTabulatedToneCurveFloat(ctx, (cmsUInt32Number)2, (const cmsFloat32Number*)values);
    curves[2] = cmsBuildTabulatedToneCurveFloat(ctx, (cmsUInt32Number)2, (const cmsFloat32Number*)values);

    const cmsCIEXYZ * d50 = cmsD50_XYZ();
    cmsCIExyY whitePoint;
    cmsXYZ2xyY(&whitePoint, d50);

    cmsHPROFILE hProfile = cmsCreateLab4ProfileTHR(ctx, &whitePoint);

    cmsSetProfileVersion(hProfile, (cmsFloat64Number)4.23);
    cmsSetEncodedICCversion(hProfile, (cmsUInt32Number)0x04300000);
    cmsSetHeaderRenderingIntent(hProfile, (cmsUInt32Number)0);
    cmsSetHeaderAttributes(hProfile, (cmsUInt64Number)0);

    cmsFloat64Number profileVersion = cmsGetProfileVersion(hProfile);
    cmsUInt32Number tagCount = cmsGetTagCount(hProfile);
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile);
    cmsColorSpaceSignature pcs = cmsGetPCS(hProfile);
    cmsContext profCtx = cmsGetProfileContextID(hProfile);

    cmsIOHANDLER * io = cmsOpenIOhandlerFromNULL(ctx);
    cmsUInt32Number wrote = cmsSaveProfileToIOhandler(hProfile, io);

    cmsUInt32Number bufSize = (cmsUInt32Number)1024;
    unsigned char buffer[1024];
    cmsBool saved = cmsSaveProfileToMem(hProfile, buffer, &bufSize);

    cmsFreeToneCurveTriple(curves);

    cmsHANDLE dict = cmsDictAlloc(ctx);

    (void)curves;
    (void)d50;
    (void)whitePoint;
    (void)hProfile;
    (void)profileVersion;
    (void)tagCount;
    (void)colorSpace;
    (void)pcs;
    (void)profCtx;
    (void)io;
    (void)wrote;
    (void)saved;
    (void)bufSize;
    (void)buffer;
    (void)dict;

    return 66;
}