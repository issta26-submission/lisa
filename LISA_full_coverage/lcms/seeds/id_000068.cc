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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_lcms_api_sequence() {
    cmsContext ctx = NULL;
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfileTHR(ctx);
    const cmsCIEXYZ * d50 = cmsD50_XYZ();
    unsigned char profileID[16];
    cmsGetHeaderProfileID(hSRGB, profileID);
    cmsSetProfileVersion(hSRGB, 4.3);
    cmsSetHeaderRenderingIntent(hSRGB, 0U);
    unsigned int bytesNeeded = 65536;
    void * mem = malloc(bytesNeeded);
    int ok_save = cmsSaveProfileToMem(hSRGB, mem, &bytesNeeded);
    cmsHPROFILE hFromMem = cmsOpenProfileFromMem(mem, bytesNeeded);
    unsigned int cs = (unsigned int) cmsGetColorSpace(hFromMem);
    int isMatrix = (int) cmsIsMatrixShaper(hFromMem);
    unsigned int manufacturer = (unsigned int) cmsGetHeaderManufacturer(hFromMem);
    unsigned int creator = (unsigned int) cmsGetHeaderCreator(hFromMem);
    free(mem);

    (void) ctx;
    (void) d50;
    (void) profileID;
    (void) ok_save;
    (void) hSRGB;
    (void) hFromMem;
    (void) cs;
    (void) isMatrix;
    (void) manufacturer;
    (void) creator;

    return 66;
}