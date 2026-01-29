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
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Declarations (use NULL for context to use the default/global context)
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hProfile2 = NULL;
    cmsUInt32Number creator = 0;
    cmsUInt32Number fmt = 0;
    cmsUInt32Number bytesNeeded = 0;
    cmsContext ctx2 = NULL;
    cmsBool md5ok = 0;
    cmsBool isclut = 0;
    cmsBool saved = 0;

    // Create a placeholder profile in the (NULL) context
    hProfile = cmsCreateProfilePlaceholder(ctx);

    // Query and modify header/manufacturer and compute ID
    creator = cmsGetHeaderCreator(hProfile);
    cmsSetHeaderManufacturer(hProfile, (cmsUInt32Number)0xABCD1234U);
    md5ok = cmsMD5computeID(hProfile);

    // Query formatter for PCS, check if profile is a CLUT profile
    fmt = cmsFormatterForPCSOfProfile(hProfile, 4U, (cmsBool)0);
    isclut = cmsIsCLUT(hProfile, 0U, 0U);

    // Ask how many bytes would be needed to save the profile (MemPtr = NULL -> get required size)
    saved = cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);

    // Retrieve the context associated with the profile and attempt to open a profile from file (may return NULL)
    ctx2 = cmsGetProfileContextID(hProfile);
    hProfile2 = cmsOpenProfileFromFile("nonexistent.icc", "r");

    // Silence unused variable warnings
    (void)ctx;
    (void)hProfile;
    (void)hProfile2;
    (void)creator;
    (void)fmt;
    (void)bytesNeeded;
    (void)ctx2;
    (void)md5ok;
    (void)isclut;
    (void)saved;

    return 66;
}