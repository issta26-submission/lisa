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
//<ID> 164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsUInt16Number curveTable[4];
    memset(curveTable, 0, sizeof(curveTable));
    curveTable[0] = (cmsUInt16Number)0;
    curveTable[1] = (cmsUInt16Number)21845;
    curveTable[2] = (cmsUInt16Number)43690;
    curveTable[3] = (cmsUInt16Number)65535;
    cmsToneCurve * curveR = cmsBuildTabulatedToneCurve16(ctx, 4U, curveTable);
    cmsToneCurve * curveG = cmsBuildTabulatedToneCurve16(ctx, 4U, curveTable);
    cmsToneCurve * curveB = cmsBuildTabulatedToneCurve16(ctx, 4U, curveTable);
    cmsToneCurve * const curves[3] = { curveR, curveG, curveB };
    cmsUInt16Number clutTable[24];
    memset(clutTable, 0, sizeof(clutTable));
    clutTable[0] = (cmsUInt16Number)0;
    clutTable[1] = (cmsUInt16Number)1;
    clutTable[2] = (cmsUInt16Number)2;
    clutTable[3] = (cmsUInt16Number)3;
    clutTable[4] = (cmsUInt16Number)4;
    clutTable[5] = (cmsUInt16Number)5;
    clutTable[6] = (cmsUInt16Number)6;
    clutTable[7] = (cmsUInt16Number)7;
    clutTable[8] = (cmsUInt16Number)8;
    clutTable[9] = (cmsUInt16Number)9;
    clutTable[10] = (cmsUInt16Number)10;
    clutTable[11] = (cmsUInt16Number)11;
    clutTable[12] = (cmsUInt16Number)12;
    clutTable[13] = (cmsUInt16Number)13;
    clutTable[14] = (cmsUInt16Number)14;
    clutTable[15] = (cmsUInt16Number)15;
    clutTable[16] = (cmsUInt16Number)16;
    clutTable[17] = (cmsUInt16Number)17;
    clutTable[18] = (cmsUInt16Number)18;
    clutTable[19] = (cmsUInt16Number)19;
    clutTable[20] = (cmsUInt16Number)20;
    clutTable[21] = (cmsUInt16Number)21;
    clutTable[22] = (cmsUInt16Number)22;
    clutTable[23] = (cmsUInt16Number)23;
    cmsStage * stage = cmsStageAllocCLut16bit(ctx, 2U, 3U, 3U, clutTable);

    // step 2: Setup
    cmsHPROFILE tmpProfile = cmsCreateXYZProfileTHR(ctx);
    cmsColorSpaceSignature cs = cmsGetColorSpace(tmpProfile);

    // step 3: Core operations
    cmsHPROFILE linProfile = cmsCreateLinearizationDeviceLink(cs, curves);
    cmsUInt32Number headerModel = cmsGetHeaderModel(linProfile);
    cmsUInt32Number inChannels = cmsStageInputChannels(stage);
    unsigned char memBuf[65536];
    memset(memBuf, 0, sizeof(memBuf));
    cmsUInt32Number bytesNeeded = (cmsUInt32Number)sizeof(memBuf);
    cmsBool saved = cmsSaveProfileToMem(linProfile, memBuf, &bytesNeeded);

    // step 4: Cleanup
    cmsStageFree(stage);
    cmsDeleteContext(ctx);

    (void)curveR;
    (void)curveG;
    (void)curveB;
    (void)curves;
    (void)clutTable;
    (void)tmpProfile;
    (void)linProfile;
    (void)headerModel;
    (void)inChannels;
    (void)memBuf;
    (void)bytesNeeded;
    (void)saved;

    // API sequence test completed successfully
    return 66;
}