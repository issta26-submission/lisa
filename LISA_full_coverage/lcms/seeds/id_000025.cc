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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Create a context and an IO handler
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsIOHANDLER * io = cmsOpenIOhandlerFromNULL(ctx);

    // step 2: Create a small in-memory sRGB profile and operate on it
    unsigned char memBuffer[64];
    memset(memBuffer, 0, sizeof memBuffer);
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);

    // step 3: Compute profile ID, read header creator and tweak header flags
    cmsBool md5computed = cmsMD5computeID(profile);
    cmsUInt32Number creator = cmsGetHeaderCreator(profile);
    cmsSetHeaderFlags(profile, (cmsUInt32Number) 0);

    // step 4: Try to save profile into our small buffer (will set required size)
    cmsUInt32Number bytesNeeded = 0;
    cmsBool saved = cmsSaveProfileToMem(profile, memBuffer, &bytesNeeded);

    // step 5: Create an XYZ profile and build a multiprofile transform with both profiles
    cmsHPROFILE xyz = cmsCreateXYZProfileTHR(ctx);
    cmsHPROFILE profiles[2];
    profiles[0] = profile;
    profiles[1] = xyz;
    cmsHTRANSFORM xform = cmsCreateMultiprofileTransformTHR(ctx, profiles, 2U, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 6: Query transform formats and delete transform
    cmsUInt32Number inFormat = cmsGetTransformInputFormat(xform);
    cmsUInt32Number outFormat = cmsGetTransformOutputFormat(xform);
    cmsDeleteTransform(xform);

    // step 7: Cleanup profiles and ignore unused results to avoid warnings
    cmsCloseProfile(xyz);
    cmsCloseProfile(profile);
    (void) io;
    (void) md5computed;
    (void) creator;
    (void) bytesNeeded;
    (void) saved;
    (void) inFormat;
    (void) outFormat;
    (void) ctx;

    // API sequence test completed successfully
    return 66;
}