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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsUInt8Number ProfileID[16] = {0};
    cmsPipeline * lut = NULL;
    cmsNAMEDCOLORLIST * ncl = NULL;
    cmsHANDLE hIT8 = NULL;
    cmsInt32Number it8Result = 0;
    cmsUInt32Number nTable = 3U;
    cmsUInt32Number inChan = 1U;
    cmsUInt32Number outChan = 1U;

    hProfile = cmsCreateProfilePlaceholder(ctx);
    cmsGetHeaderProfileID(hProfile, ProfileID);
    inChan = (cmsUInt32Number)((ProfileID[0] % 5U) + 1U);
    outChan = (cmsUInt32Number)((ProfileID[1] % 5U) + 1U);
    lut = cmsPipelineAlloc(ctx, inChan, outChan);
    hIT8 = cmsIT8Alloc(ctx);
    it8Result = cmsIT8SetTableByLabel(hIT8, "DefaultSet", "DefaultField", "Type");
    cmsPipelineFree(lut);
    cmsIT8Free(hIT8);
    cmsFreeNamedColorList(ncl);

    (void)ctx;
    (void)hProfile;
    (void)ProfileID;
    (void)nTable;
    (void)inChan;
    (void)outChan;
    (void)it8Result;

    return 66;
}