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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE devlink = NULL;
    cmsHTRANSFORM xform = NULL;
    cmsToneCurve * tone = NULL;
    cmsUInt8Number profileID[16] = {0};
    cmsContext pipelineCtx = NULL;
    cmsUInt32Number combined = 0;
    cmsFloat64Number version = 0.0;
    cmsBool isClut = 0;
    cmsUInt32Number inFmt = 0, outFmt = 0;
    cmsFloat64Number tac = 0.0;
    cmsUInt32Number intent = 0;

    hProfile = cmsCreateXYZProfileTHR(ctx);
    version = cmsGetProfileVersion(hProfile);
    cmsGetHeaderProfileID(hProfile, profileID);
    xform = cmsCreateTransform(hProfile, 0U, hProfile, 0U, 0U, 0U);
    devlink = cmsTransform2DeviceLink(xform, version, 0U);
    inFmt = cmsGetTransformInputFormat(xform);
    outFmt = cmsGetTransformOutputFormat(xform);
    cmsChangeBuffersFormat(xform, inFmt, outFmt);
    tac = cmsDetectTAC(hProfile);
    isClut = cmsIsCLUT(hProfile, 0U, 0U);
    intent = cmsGetHeaderRenderingIntent(hProfile);
    cmsSetHeaderProfileID(hProfile, profileID);
    cmsSetHeaderRenderingIntent(hProfile, intent);
    pipelineCtx = cmsGetProfileContextID(hProfile);
    combined = (cmsUInt32Number)profileID[0] + (cmsUInt32Number)isClut + (cmsUInt32Number)((int)version);

    cmsCloseProfile(devlink);
    cmsCloseProfile(hProfile);

    (void)ctx;
    (void)tone;
    (void)pipelineCtx;
    (void)combined;
    (void)tac;
    (void)inFmt;
    (void)outFmt;

    return 66;
}