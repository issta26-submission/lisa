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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsUInt16Number table4[4];
    memset(table4, 0, sizeof(table4));
    table4[0] = (cmsUInt16Number)0;
    table4[1] = (cmsUInt16Number)21845;   // ~1/3 of 65535
    table4[2] = (cmsUInt16Number)43690;   // ~2/3 of 65535
    table4[3] = (cmsUInt16Number)65535;   // max

    cmsToneCurve * c1 = cmsBuildTabulatedToneCurve16(ctx, 4U, table4);
    cmsToneCurve * c2 = cmsDupToneCurve(c1);
    cmsToneCurve * c3 = cmsReverseToneCurve(c1);
    cmsToneCurve * transfer[3];
    transfer[0] = c1;
    transfer[1] = c2;
    transfer[2] = c3;

    unsigned char memBuf[1024];
    memset(memBuf, 0, sizeof(memBuf));
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)sizeof(memBuf);

    // step 2: Setup
    cmsHPROFILE devLink = cmsCreateLinearizationDeviceLink((cmsColorSpaceSignature)0x52474220, transfer);

    // step 3: Core operations
    cmsUInt32Number headerModel = cmsGetHeaderModel(devLink);
    cmsStage * stage = cmsStageAllocCLut16bit(ctx, 2U, 3U, 3U, table4);
    cmsUInt32Number inputChannels = cmsStageInputChannels(stage);
    cmsBool saved = cmsSaveProfileToMem(devLink, memBuf, &bytesNeeded);

    // step 4: Cleanup & Validate
    cmsStageFree(stage);

    (void)ctx;
    (void)table4;
    (void)c1;
    (void)c2;
    (void)c3;
    (void)transfer;
    (void)devLink;
    (void)headerModel;
    (void)inputChannels;
    (void)saved;
    (void)memBuf;
    (void)bytesNeeded;

    // API sequence test completed successfully
    return 66;
}