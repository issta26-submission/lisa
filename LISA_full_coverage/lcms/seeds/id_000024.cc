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
//<ID> 24
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations
    cmsContext ctx = (cmsContext) NULL;
    cmsIOHANDLER * io = (cmsIOHANDLER *) NULL;
    cmsHPROFILE profile = (cmsHPROFILE) NULL;
    cmsStage * stage = (cmsStage *) NULL;
    cmsUInt16Number clutTable[8];
    cmsUInt32Number inputChannels = 0;
    cmsUInt32Number wideCount = 0;
    wchar_t wideBuffer[64];
    char lang[3] = { 'e', 'n', '\0' };
    char country[3] = { 'U', 'S', '\0' };
    unsigned char dummyProfileMem[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

    // step 2: Setup
    io = cmsOpenIOhandlerFromNULL(ctx);
    profile = cmsOpenProfileFromMem((const void *) dummyProfileMem, (cmsUInt32Number) sizeof(dummyProfileMem));
    clutTable[0] = (cmsUInt16Number) 0;
    clutTable[1] = (cmsUInt16Number) 32768;
    clutTable[2] = (cmsUInt16Number) 65535;
    clutTable[3] = (cmsUInt16Number) 16384;
    clutTable[4] = (cmsUInt16Number) 8192;
    clutTable[5] = (cmsUInt16Number) 24576;
    clutTable[6] = (cmsUInt16Number) 4096;
    clutTable[7] = (cmsUInt16Number) 61440;

    // step 3: Core operations
    stage = cmsStageAllocCLut16bit(ctx, 2U, 3U, 3U, clutTable);
    inputChannels = cmsStageInputChannels(stage);
    wideCount = cmsMLUgetWide((const cmsMLU *) NULL, lang, country, wideBuffer, (cmsUInt32Number) (sizeof(wideBuffer) / sizeof(wideBuffer[0])));

    // step 4: Cleanup
    memset(wideBuffer, 0, sizeof(wideBuffer));
    (void) io;
    (void) profile;
    (void) stage;
    (void) inputChannels;
    (void) wideCount;
    // API sequence test completed successfully
    return 66;
}