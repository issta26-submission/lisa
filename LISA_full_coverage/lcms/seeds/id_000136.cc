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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsHANDLE hIT8 = NULL;
    cmsPipeline * lut = NULL;
    cmsNAMEDCOLORLIST * ncl = NULL;
    cmsUInt8Number profileID[16];
    cmsUInt32Number inChan = 3U;
    cmsUInt32Number outChan = 3U;
    cmsUInt32Number nTable = 0U;
    cmsInt32Number setRes = 0;

    // step 2: Setup
    memset(profileID, 0, sizeof(profileID));
    hProfile = cmsCreateProfilePlaceholder(ctx);
    cmsGetHeaderProfileID(hProfile, profileID);
    nTable = (cmsUInt32Number) profileID[0];
    hIT8 = cmsGBDAlloc(ctx);

    // step 3: Core operations
    setRes = (cmsInt32Number) cmsGetHeaderCreator(hProfile);
    ncl = cmsAllocNamedColorList(ctx, 1U, 3U, "P", "S");
    setRes = setRes + (cmsInt32Number) cmsNamedColorCount(ncl);
    inChan = inChan + (cmsUInt32Number)(setRes & 0x3);
    outChan = outChan + (cmsUInt32Number)((setRes >> 2) & 0x3);

    // step 4: Cleanup
    cmsGBDFree(hIT8);

    (void)ctx;
    (void)hProfile;
    (void)lut;
    (void)profileID;
    (void)setRes;
    (void)inChan;
    (void)outChan;
    (void)nTable;
    (void)ncl;
    // API sequence test completed successfully
    return 66;
}