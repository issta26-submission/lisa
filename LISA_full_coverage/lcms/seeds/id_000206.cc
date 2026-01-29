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
//<ID> 206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    char * buffer = (char *)malloc(256);
    cmsIOHANDLER * io = cmsOpenIOhandlerFromMem((cmsContext)NULL, buffer, 256, "w+");
    cmsHPROFILE hProfile = cmsCreateNULLProfile();
    cmsUInt16Number alarmCodes[16];
    memset(alarmCodes, 0, sizeof(alarmCodes));
    alarmCodes[0] = (cmsUInt16Number)0x0101;
    alarmCodes[1] = (cmsUInt16Number)0x0202;
    alarmCodes[2] = (cmsUInt16Number)0x0303;

    // step 2: Configure
    cmsContext ctx = cmsDupContext((cmsContext)NULL, NULL);
    cmsSetHeaderRenderingIntent(hProfile, 0);

    // step 3: Operate
    cmsFloat64Number version = cmsGetProfileVersion(hProfile);
    cmsUInt32Number flags = (cmsUInt32Number)(unsigned long)(version);
    cmsSetHeaderFlags(hProfile, flags);
    cmsMD5computeID(hProfile);

    // step 4: Validate & Cleanup
    cmsCloseIOhandler(io);
    free(buffer);
    cmsDeleteContext(ctx);

    (void) alarmCodes;
    (void) version;
    (void) flags;
    (void) hProfile;
    (void) io;
    (void) ctx;

    // API sequence test completed successfully
    return 66;
}