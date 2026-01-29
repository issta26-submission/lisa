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
//<ID> 1451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for inflateBackInit_/compress/inflateCodesUsed";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong compBound = compressBound(srcLen);
    uLongf compDestLen = (uLongf)compBound;
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Setup - compress source into buffer
    int rc_compress = compress(compBuf, &compDestLen, src, srcLen);
    uLong compLen = (uLong)compDestLen;

    // step 3: Initialize inflate-back structure
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    unsigned char *window = new unsigned char[(1 << 15)];
    memset(window, 0, (size_t)(1 << 15));
    const char *version = zlibVersion();
    int rc_inflate_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    // step 4: Validate & Cleanup
    unsigned long codes_used = inflateCodesUsed(&strm);
    int rc_inflate_end = inflateBackEnd(&strm);
    delete [] src;
    delete [] compBuf;
    delete [] window;
    (void)rc_compress;
    (void)compLen;
    (void)version;
    (void)rc_inflate_init;
    (void)codes_used;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}