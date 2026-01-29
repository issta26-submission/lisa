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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Declarations
    cmsContext ctx = NULL;
    cmsHPROFILE nullProf = NULL;
    cmsHPROFILE grayProf = NULL;
    cmsToneCurve * tone = NULL;
    cmsIOHANDLER * io = NULL;
    cmsCIExyY whitePoint;
    cmsUInt16Number tbl[3];
    cmsBool md5ok = 0;
    cmsUInt32Number formatterSize = 0;
    cmsUInt32Number savedBytes = 0;

    // Step 2: Setup
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    tbl[0] = (cmsUInt16Number)0x0000;
    tbl[1] = (cmsUInt16Number)0x8000;
    tbl[2] = (cmsUInt16Number)0xFFFF;

    // Step 3: Core operations
    tone = cmsBuildTabulatedToneCurve16(ctx, 3U, tbl);
    grayProf = cmsCreateGrayProfileTHR(ctx, &whitePoint, tone);
    md5ok = cmsMD5computeID(grayProf);
    formatterSize = cmsFormatterForPCSOfProfile(grayProf, 32U, (cmsBool)0);
    io = cmsOpenIOhandlerFromNULL(ctx);
    savedBytes = cmsSaveProfileToIOhandler(grayProf, io);

    // Step 4: Cleanup
    cmsUnregisterPluginsTHR(ctx);

    (void)ctx;
    (void)nullProf;
    (void)tone;
    (void)io;
    (void)whitePoint;
    (void)tbl;
    (void)md5ok;
    (void)formatterSize;
    (void)savedBytes;
    (void)grayProf;

    return 66;
}
// API sequence test completed successfully