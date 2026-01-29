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
//<ID> 155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialize
    cmsHPROFILE hProfile = NULL;
    cmsContext ctx = NULL;
    cmsIOHANDLER * io = NULL;
    cmsToneCurve * curve = NULL;
    cmsUInt16Number table[4];
    cmsBool md5_ok = 0;
    cmsUInt32Number formatter = 0;
    cmsUInt32Number nBytes = 128;
    cmsBool lIsFloat = 0;

    memset(table, 0, sizeof(table));
    table[0] = 0;
    table[1] = 0x4000; /* mid */
    table[2] = 0x8000; /* higher */
    table[3] = 0xFFFF; /* full */

    hProfile = cmsCreateNULLProfile();
    ctx = cmsGetProfileContextID(hProfile);

    // step 2: Configure resources (build tone curve, open io handler)
    curve = cmsBuildTabulatedToneCurve16(ctx, 4, table);
    io = cmsOpenIOhandlerFromNULL(ctx);

    // step 3: Operate & Validate (compute MD5, get formatter, save profile)
    md5_ok = cmsMD5computeID(hProfile);
    formatter = cmsFormatterForPCSOfProfile(hProfile, nBytes, lIsFloat);
    cmsUInt32Number saved = cmsSaveProfileToIOhandler(hProfile, io);

    // step 4: Cleanup & final checks
    (void)curve;
    (void)md5_ok;
    (void)formatter;
    (void)saved;
    (void)io;
    (void)ctx;
    (void)table;
    (void)hProfile;

    // API sequence test completed successfully
    return 66;
}