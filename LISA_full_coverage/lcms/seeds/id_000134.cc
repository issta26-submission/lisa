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
//<ID> 134
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations and initialization
    cmsContext ctx = NULL;
    cmsHPROFILE hProfile = cmsCreateXYZProfile();
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    cmsMLU * mlu = cmsMLUalloc(ctx, (cmsUInt32Number)1);
    cmsNAMEDCOLORLIST * named = cmsAllocNamedColorList(ctx, (cmsUInt32Number)1, (cmsUInt32Number)4, "Pfx", "Sfx");
    cmsUInt8Number ProfileID[16] = {0};
    unsigned char membuf[64] = {0};
    char asciiBuf[64] = {0};
    cmsUInt32Number BytesNeeded = (cmsUInt32Number)0;
    cmsUInt32Number model = cmsGetHeaderModel(hProfile);

    // step 2: Configuration / data population (straight-line, no branches)
    cmsSetHeaderModel(hProfile, model);
    cmsIT8SetDataFormat(it8, 1, "RGB");
    cmsIT8SetDataDbl(it8, "Patch1", "R", (cmsFloat64Number)0.5);
    cmsIT8SaveToMem(it8, membuf, &BytesNeeded);
    cmsMLUsetASCII(mlu, "eng", "USA", "ASCII");
    cmsMLUgetASCII(mlu, "eng", "USA", asciiBuf, (cmsUInt32Number)sizeof(asciiBuf));

    // step 3: Cleanup using available APIs
    cmsMLUfree(mlu);
    cmsFreeNamedColorList(named);
    cmsCloseProfile(hProfile);

    // avoid unused variable warnings
    (void)ctx;
    (void)it8;
    (void)ProfileID;
    (void)BytesNeeded;
    (void)model;
    (void)membuf;
    (void)asciiBuf;

    // API sequence test completed successfully
    return 66;
}