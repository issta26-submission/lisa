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
//<ID> 292
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsContext dupCtx = cmsDupContext(ctx, NULL);
    cmsSetLogErrorHandlerTHR(ctx, NULL);

    cmsHPROFILE prof1 = cmsCreate_sRGBProfile();
    cmsHPROFILE prof2 = cmsCreate_OkLabProfile(ctx);

    cmsFloat64Number version = cmsGetProfileVersion(prof1);
    cmsUInt32Number creator = cmsGetHeaderCreator(prof1);

    cmsUInt32Number intents[4];
    cmsUInt32Number nIntents = cmsGetSupportedIntents(4, intents, (char **)NULL);

    cmsToneCurve * gamma = cmsBuildGamma(ctx, 2.2);
    cmsUInt32Number estEntries = cmsGetToneCurveEstimatedTableEntries(gamma);
    cmsBool isMulti = cmsIsToneCurveMultisegment(gamma);
    cmsToneCurve * rev = cmsReverseToneCurveEx(256, gamma);
    cmsFreeToneCurve(gamma);
    cmsFreeToneCurve(rev);

    cmsPipeline * pipe = cmsPipelineAlloc(ctx, 3, 3);
    cmsPipelineSetSaveAs8bitsFlag(pipe, 1);
    cmsPipeline * pipeDup = cmsPipelineDup(pipe);
    cmsBool catResult = cmsPipelineCat(pipe, pipeDup);
    cmsUInt32Number outCh = cmsPipelineOutputChannels(pipe);

    cmsStage * stage = cmsStageAllocIdentity(ctx, outCh);
    cmsContext stageCtx = cmsGetStageContextID(stage);
    cmsStageFree(stage);

    cmsContext pipeCtx = cmsGetPipelineContextID(pipe);
    int cmmVer = cmsGetEncodedCMMversion();

    cmsUnregisterPlugins();
    void * userData = cmsGetContextUserData(ctx);

    (void)dupCtx; (void)version; (void)creator; (void)nIntents;
    (void)estEntries; (void)isMulti; (void)catResult; (void)outCh;
    (void)stageCtx; (void)pipeCtx; (void)cmmVer; (void)userData;

    return 66;
}