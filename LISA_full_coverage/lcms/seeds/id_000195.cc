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
//<ID> 195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsHPROFILE hNullProfile = cmsCreateNULLProfileTHR((cmsContext)NULL);
    cmsSetHeaderModel(hNullProfile, (cmsUInt32Number)0x4E554C4C);

    cmsHPROFILE hXYZProfile = cmsCreateXYZProfileTHR((cmsContext)NULL);

    cmsToneCurve * gamma = cmsBuildGamma((cmsContext)NULL, 2.2);
    cmsToneCurve * transferFunctions[3] = { gamma, gamma, gamma };
    cmsHPROFILE hRGBProfile = cmsCreateRGBProfileTHR((cmsContext)NULL,
                                                     (const cmsCIExyY *)NULL,
                                                     (const cmsCIExyYTRIPLE *)NULL,
                                                     transferFunctions);

    cmsPipeline * pipe = cmsPipelineAlloc((cmsContext)NULL, (cmsUInt32Number)3, (cmsUInt32Number)3);
    cmsStage * identityStage = cmsStageAllocIdentity((cmsContext)NULL, (cmsUInt32Number)3);

    cmsUInt32Number stageCount = cmsPipelineStageCount(pipe);
    cmsUInt32Number pipelineOutCh = cmsPipelineOutputChannels(pipe);
    cmsStage * lastStagePtr = cmsPipelineGetPtrToLastStage(pipe);
    cmsUInt32Number stageOutCh = cmsStageOutputChannels(identityStage);
    void * stageDat = cmsStageData(identityStage);

    cmsUInt16Number in[3];
    cmsUInt16Number out[3];
    in[0] = (cmsUInt16Number)0;
    in[1] = (cmsUInt16Number)32768;
    in[2] = (cmsUInt16Number)65535;
    out[0] = (cmsUInt16Number)0;
    out[1] = (cmsUInt16Number)0;
    out[2] = (cmsUInt16Number)0;

    cmsPipelineEval16(in, out, pipe);

    (void)hNullProfile;
    (void)hXYZProfile;
    (void)hRGBProfile;
    (void)gamma;
    (void)transferFunctions;
    (void)pipe;
    (void)identityStage;
    (void)stageCount;
    (void)pipelineOutCh;
    (void)lastStagePtr;
    (void)stageOutCh;
    (void)stageDat;
    (void)in;
    (void)out;

    return 66;
}