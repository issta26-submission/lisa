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
//<ID> 168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsUInt16Number linvals1[4] = {0, 21845, 43690, 65535};
    cmsUInt16Number linvals2[4] = {0, 16384, 49152, 65535};
    cmsToneCurve * curve1 = NULL;
    cmsToneCurve * curve2 = NULL;
    cmsToneCurve * curves[2] = { NULL, NULL };
    cmsColorSpaceSignature cs = (cmsColorSpaceSignature)0x52474220; /* 'RGB ' */
    cmsHPROFILE profile = NULL;
    cmsUInt32Number headerModel = 0;
    cmsUInt32Number needed = 0;
    void * mem = NULL;
    cmsBool ok = 0;
    cmsStage * stage = NULL;
    cmsUInt16Number clutTable[24];
    memset(clutTable, 0, sizeof(clutTable));

    // step 2: Setup
    curve1 = cmsBuildTabulatedToneCurve16((cmsContext)NULL, 4U, linvals1);
    curve2 = cmsBuildTabulatedToneCurve16((cmsContext)NULL, 4U, linvals2);
    /* prepare array of transfer functions for linearization device link */
    curves[0] = curve1;
    curves[1] = curve2;
    profile = cmsCreateLinearizationDeviceLink(cs, (cmsToneCurve * const *)curves);

    // step 3: Core operations
    headerModel = cmsGetHeaderModel(profile);
    stage = cmsStageAllocCLut16bit((cmsContext)NULL, 2U, 3U, 3U, clutTable);
    cmsUInt32Number stageInputs = cmsStageInputChannels((const cmsStage *)stage);
    cmsStageFree(stage);
    cmsSaveProfileToMem(profile, NULL, &needed);
    mem = malloc((size_t)needed);
    ok = cmsSaveProfileToMem(profile, mem, &needed);

    // step 4: Cleanup
    free(mem);
    (void)curve1;
    (void)curve2;
    (void)curves;
    (void)cs;
    (void)profile;
    (void)headerModel;
    (void)stageInputs;
    (void)needed;
    (void)ok;
    (void)clutTable;

    // API sequence test completed successfully
    return 66;
}