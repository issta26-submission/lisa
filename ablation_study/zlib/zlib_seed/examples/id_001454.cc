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
//<ID> 1454
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
    const char *version = zlibVersion();

    // step 2: Setup - compute compression bound and compress the source
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    uLongf compDestLen = (uLongf)compBound;
    int rc_compress = compress(compBuf, &compDestLen, src, srcLen);
    uLong compLen = (uLong)compDestLen;

    // step 3: Initialize inflateBack, query codes used, and tear down the back inflater
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char *window = new unsigned char[(1 << 15)];
    memset(window, 0, (size_t)(1 << 15));
    int rc_inflate_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));
    unsigned long codes_used = inflateCodesUsed(&strm);
    int rc_inflate_end = inflateBackEnd(&strm);

    // step 4: Cleanup
    delete [] src;
    delete [] compBuf;
    delete [] window;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)codes_used;
    (void)rc_inflate_end;
    (void)compLen;
    (void)version;
    // API sequence test completed successfully
    return 66;
}