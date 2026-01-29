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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Declarations
    cmsHPROFILE hProfile = NULL;
    cmsContext ctx = NULL;
    unsigned char profileID[16] = {0};
    cmsPipeline * lut = NULL;
    cmsUInt32Number stageCount = 0;
    cmsStage * firstStage = NULL;
    cmsStage * lastStage = NULL;
    cmsHANDLE dict = NULL;
    const cmsDICTentry * dictEntries = NULL;
    cmsHANDLE it8 = NULL;
    cmsFloat64Number propVal = 1.234;
    cmsUInt32Number bytesNeeded = 0;
    cmsFloat64Number version = 0.0;
    cmsUInt32Number creator = 0;
    struct tm creation = {0};
    cmsBool gotDate = (cmsBool)0;
    cmsBool saved = (cmsBool)0;

    // Sequence of API calls (straight-line)
    hProfile = cmsCreateNULLProfile();
    ctx = cmsGetProfileContextID(hProfile);
    cmsGetHeaderProfileID(hProfile, profileID);
    lut = cmsPipelineAlloc(ctx, 3U, 3U);
    stageCount = cmsPipelineStageCount(lut);
    firstStage = cmsPipelineGetPtrToFirstStage(lut);
    lastStage = cmsPipelineGetPtrToLastStage(lut);
    cmsPipelineFree(lut);
    dict = cmsDictAlloc(ctx);
    dictEntries = cmsDictGetEntryList(dict);
    it8 = cmsIT8Alloc(ctx);
    cmsIT8SetPropertyDbl(it8, "ExampleProp", propVal);
    saved = cmsIT8SaveToMem(it8, NULL, &bytesNeeded);
    version = cmsGetProfileVersion(hProfile);
    creator = cmsGetHeaderCreator(hProfile);
    cmsSetHeaderManufacturer(hProfile, creator);
    gotDate = cmsGetHeaderCreationDateTime(hProfile, &creation);

    (void)ctx;
    (void)profileID;
    (void)stageCount;
    (void)firstStage;
    (void)lastStage;
    (void)dictEntries;
    (void)bytesNeeded;
    (void)version;
    (void)creator;
    (void)creation;
    (void)gotDate;
    (void)saved;
    (void)it8;
    (void)dict;
    (void)hProfile;
    (void)lut;
    (void)propVal;

    return 66;
}