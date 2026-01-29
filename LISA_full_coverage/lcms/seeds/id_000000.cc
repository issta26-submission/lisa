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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsHPROFILE profile = cmsCreateNULLProfileTHR(ctx);
    cmsUInt32Number encVersion = cmsGetEncodedICCversion(profile);
    (void)encVersion;
    cmsUInt32Number headerFlags = cmsGetHeaderFlags(profile);
    (void)headerFlags;
    cmsUInt32Number headerCreator = cmsGetHeaderCreator(profile);
    (void)headerCreator;
    cmsUInt32Number headerModel = cmsGetHeaderModel(profile);
    (void)headerModel;
    cmsUInt32Number renderingIntent = cmsGetHeaderRenderingIntent(profile);
    (void)renderingIntent;
    cmsUInt32Number bytesBuf = 1024;
    void * buffer = malloc((size_t)bytesBuf);
    int saved = cmsSaveProfileToMem(profile, buffer, &bytesBuf);
    (void)saved;
    cmsHPROFILE profile_from_mem = cmsOpenProfileFromMem(buffer, bytesBuf);
    (void)cmsGetEncodedICCversion(profile_from_mem);
    free(buffer);
    (void)ctx;
    (void)profile;
    (void)profile_from_mem;
    return 66;
}