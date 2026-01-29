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
//<ID> 209
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsUInt16Number alarms[16];
    memset(alarms, 0, sizeof(alarms));
    alarms[0] = (cmsUInt16Number)0x0101U;
    alarms[1] = (cmsUInt16Number)0x0202U;

    unsigned char membuf[128] = {0};
    cmsIOHANDLER * io = cmsOpenIOhandlerFromMem(ctx, membuf, sizeof(membuf), "w+");
    cmsHPROFILE hProfile = cmsCreateNULLProfileTHR(ctx);

    // step 2: Configure
    cmsSetAlarmCodesTHR(ctx, alarms);

    // step 3: Operate
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);
    cmsFloat64Number tac = (cmsFloat64Number)intent * 1.0; // derive a double-like value from an available API
    cmsUInt16Number scaled = (cmsUInt16Number)(tac * 100.0);
    alarms[2] = scaled;
    cmsSetAlarmCodesTHR(ctx, alarms);

    // step 4: Validate & Cleanup
    cmsCloseIOhandler(io);
    cmsDeleteContext(ctx);

    (void)intent;
    (void)tac;
    (void)scaled;
    (void)alarms;
    (void)io;
    (void)hProfile;
    // API sequence test completed successfully
    return 66;
}