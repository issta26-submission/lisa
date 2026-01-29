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
//<ID> 166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number colorSpace = 0;
    cmsUInt32Number pcs = 0;
    cmsUInt32Number headerModel = 0;
    cmsUInt32Number creator = 0;
    cmsUInt32Number renderingIntent = 0;
    cmsUInt32Number encodedICCversion = 0;
    int cmmVersion = 0;
    cmsBool md5ok = 0;
    cmsBool closed = 0;

    hProfile = cmsCreateNULLProfile();
    colorSpace = cmsGetColorSpace(hProfile);
    pcs = cmsGetPCS(hProfile);
    headerModel = cmsGetHeaderModel(hProfile);
    creator = cmsGetHeaderCreator(hProfile);
    renderingIntent = cmsGetHeaderRenderingIntent(hProfile);
    encodedICCversion = cmsGetEncodedICCversion(hProfile);
    cmmVersion = cmsGetEncodedCMMversion();
    md5ok = cmsMD5computeID(hProfile);
    closed = cmsCloseProfile(hProfile);

    (void) colorSpace;
    (void) pcs;
    (void) headerModel;
    (void) creator;
    (void) renderingIntent;
    (void) encodedICCversion;
    (void) cmmVersion;
    (void) md5ok;
    (void) closed;

    return 66;
}