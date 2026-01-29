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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = (cmsContext) NULL;
    cmsIOHANDLER * io = cmsOpenIOhandlerFromNULL(ctx);
    unsigned char sampleProfileData[8] = { 0x00, 0x01, 0x02, 0x10, 0x20, 0x30, 0x40, 0x50 };
    cmsUInt32Number profileSize = (cmsUInt32Number) sizeof(sampleProfileData);
    cmsHPROFILE profile = cmsOpenProfileFromMem(sampleProfileData, profileSize);
    cmsUInt16Number clutTable[8];
    memset(clutTable, 0, sizeof(clutTable));
    cmsStage * stage = cmsStageAllocCLut16bit(ctx, 2U, 3U, 3U, clutTable);

    // step 2: Configure
    cmsUInt32Number inputChannels = cmsStageInputChannels(stage);
    wchar_t wideBuffer[64];
    memset(wideBuffer, 0, sizeof(wideBuffer));
    cmsUInt32Number obtained = cmsMLUgetWide((const cmsMLU *) NULL, "eng", "USA", wideBuffer, (cmsUInt32Number)(sizeof(wideBuffer) / sizeof(wchar_t)));

    // step 3: Operate
    void * stageData = cmsStageData(stage);
    (void) io;
    (void) profile;
    (void) stageData;
    (void) inputChannels;
    (void) obtained;

    // step 4: Cleanup
    memset(clutTable, 0, sizeof(clutTable));
    memset(wideBuffer, 0, sizeof(wideBuffer));
    return 66;
    // API sequence test completed successfully
}