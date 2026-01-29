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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE hProfile = cmsCreateNULLProfile();
    cmsContext ctx = cmsGetProfileContextID(hProfile);
    cmsIOHANDLER * io = cmsOpenIOhandlerFromNULL(ctx);
    cmsUInt16Number table[4];
    memset(table, 0, sizeof(table));
    table[0] = (cmsUInt16Number)0;
    table[1] = (cmsUInt16Number)21845;   // ~1/3 of 65535
    table[2] = (cmsUInt16Number)43690;   // ~2/3 of 65535
    table[3] = (cmsUInt16Number)65535;   // max

    // step 2: Configure (build tone curve from table)
    cmsToneCurve * curve = cmsBuildTabulatedToneCurve16(ctx, 4U, table);

    // step 3: Operate (format/profile operations and compute ID)
    cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(hProfile, 128U, 1 /* lIsFloat */);
    cmsBool md5ok = cmsMD5computeID(hProfile);
    cmsUInt32Number savedBytes = cmsSaveProfileToIOhandler(hProfile, io);

    // step 4: Validate & Cleanup
    cmsContext profCtx = cmsGetProfileContextID(hProfile);

    (void)curve;
    (void)formatter;
    (void)md5ok;
    (void)savedBytes;
    (void)profCtx;
    (void)table;
    (void)io;
    (void)ctx;
    (void)hProfile;

    // API sequence test completed successfully
    return 66;
}