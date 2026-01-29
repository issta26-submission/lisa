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
//<ID> 1450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "inflateBackInit_ and compressBound integration payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));

    // step 2: Setup
    uLong compBound = compressBound(srcLen);
    uLongf compDestLen = (uLongf)compBound;
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    int rc_compress = compress(compBuf, &compDestLen, src, srcLen);
    unsigned char *window = new unsigned char[32768];
    memset(window, 0, 32768);

    // step 3: Core operations
    int rc_inflate_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));
    unsigned long codes_used = inflateCodesUsed(&strm);
    int rc_inflate_end = inflateBackEnd(&strm);

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf;
    delete [] window;
    (void)version;
    (void)compBound;
    (void)compDestLen;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)codes_used;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}