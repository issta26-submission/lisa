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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Declarations
    cmsContext ctx = (cmsContext) NULL;
    cmsHPROFILE hProfile = cmsCreateXYZProfile();
    cmsFloat64Number version = 0.0;
    cmsUInt32Number encodedVersion = 0;
    cmsInt32Number tagCount = 0;
    cmsUInt32Number bytesNeeded = 0;
    struct tm creation;
    cmsBool isMatrix = 0;
    cmsBool intentSupported = 0;
    cmsCIEXYZ blackPointDetect;
    cmsCIEXYZ destBlackPointDetect;
    cmsBool saved = 0;

    // Straight-line sequence of API calls using the profile
    version = cmsGetProfileVersion(hProfile);
    cmsSetEncodedICCversion(hProfile, (cmsUInt32Number) version);
    encodedVersion = cmsGetEncodedICCversion(hProfile);
    tagCount = cmsGetTagCount(hProfile);
    cmsGetHeaderCreationDateTime(hProfile, &creation);
    isMatrix = cmsIsMatrixShaper(hProfile);
    intentSupported = cmsIsIntentSupported(hProfile, 0U, 0U);
    cmsDetectBlackPoint(&blackPointDetect, hProfile, 0U, 0U);
    cmsDetectDestinationBlackPoint(&destBlackPointDetect, hProfile, 0U, 0U);
    saved = cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
    cmsCloseProfile(hProfile);

    // Silence unused-variable warnings in a single-line manner
    (void) ctx;
    (void) version;
    (void) encodedVersion;
    (void) tagCount;
    (void) creation;
    (void) isMatrix;
    (void) intentSupported;
    (void) blackPointDetect;
    (void) destBlackPointDetect;
    (void) saved;
    (void) bytesNeeded;

    return 66;
}