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
//<ID> 165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsUInt16Number toneTable[4];
    cmsUInt16Number clutTable[24];
    cmsUInt32Number clutGridPoints = 2;
    cmsStage * stage = NULL;
    cmsUInt32Number stageInputCh = 0;
    cmsToneCurve * curveR = NULL;
    cmsToneCurve * curveG = NULL;
    cmsToneCurve * curveB = NULL;
    cmsToneCurve * curves[3];
    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number needed = 0;
    void * mem = NULL;
    cmsUInt32Number headerModel = 0;
    cmsBool savedOk = 0;

    // step 2: Setup (initialize tables and build small CLUT)
    memset(toneTable, 0, sizeof(toneTable));
    toneTable[0] = (cmsUInt16Number)0;
    toneTable[1] = (cmsUInt16Number)21845;   // ~1/3 of 65535
    toneTable[2] = (cmsUInt16Number)43690;   // ~2/3 of 65535
    toneTable[3] = (cmsUInt16Number)65535;   // max

    memset(clutTable, 0, sizeof(clutTable)); // small zero CLUT (2^3 * 3 entries = 24)

    // step 3: Core operations (allocate a stage, inspect it, free it; build curves; create profile; save to memory)
    stage = cmsStageAllocCLut16bit(NULL, clutGridPoints, 3U, 3U, clutTable);
    stageInputCh = cmsStageInputChannels(stage);
    cmsStageFree(stage);

    curveR = cmsBuildTabulatedToneCurve16(NULL, 4U, toneTable);
    curveG = cmsBuildTabulatedToneCurve16(NULL, 4U, toneTable);
    curveB = cmsBuildTabulatedToneCurve16(NULL, 4U, toneTable);
    curves[0] = curveR;
    curves[1] = curveG;
    curves[2] = curveB;

    hProfile = cmsCreateLinearizationDeviceLink((cmsColorSpaceSignature)0x52474220 /* 'RGB ' */, curves);
    headerModel = cmsGetHeaderModel(hProfile);

    cmsSaveProfileToMem(hProfile, NULL, &needed);
    mem = malloc((size_t)needed);
    savedOk = cmsSaveProfileToMem(hProfile, mem, &needed);
    free(mem);

    // step 4: Cleanup & finalization
    (void)curveR;
    (void)curveG;
    (void)curveB;
    (void)curves;
    (void)hProfile;
    (void)stageInputCh;
    (void)headerModel;
    (void)savedOk;
    (void)needed;
    (void)clutTable;
    (void)toneTable;

    // API sequence test completed successfully
    return 66;
}