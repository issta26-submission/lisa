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
//<ID> 1452
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "example payload for zlib inflateBackInit_/compress/inflateCodesUsed sequence";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    const char *version = zlibVersion();

    // step 2: Setup
    uLongf destLen = compBound;
    int rc_compress = compress(compBuf, &destLen, src, srcLen);
    uLong compLen = (uLong)destLen;

    // step 3: Core operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = NULL;
    strm.avail_out = 0;
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    unsigned char *window = new unsigned char[(size_t)(1 << 15)];
    memset(window, 0, (size_t)(1 << 15));
    int rc_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));
    unsigned long codes_used = inflateCodesUsed(&strm);
    int rc_backend = inflateBackEnd(&strm);

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf;
    delete [] window;
    (void)rc_compress;
    (void)rc_init;
    (void)codes_used;
    (void)rc_backend;
    (void)version;
    // API sequence test completed successfully
    return 66;
}