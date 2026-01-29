#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "payload for inflateBackInit_ and compressBound test";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    memcpy(compBuf, payload, (size_t)srcLen);

    // step 2: Setup
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const int windowBits = 15;
    const int windowSize = 1 << windowBits;
    unsigned char *window = new unsigned char[(size_t)windowSize];
    memset(window, 0, (size_t)windowSize);

    // step 3: Core operations
    int rc_init = inflateBackInit_(&strm, windowBits, window, version, (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)srcLen;
    unsigned long used_codes = inflateCodesUsed(&strm);

    // step 4: Validate & Cleanup
    int rc_end = inflateBackEnd(&strm);
    delete [] compBuf;
    delete [] window;
    (void)version;
    (void)rc_init;
    (void)used_codes;
    (void)rc_end;
    // API sequence test completed successfully
    return 66;
}