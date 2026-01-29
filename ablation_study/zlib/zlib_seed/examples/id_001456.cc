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
//<ID> 1456
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

    // step 2: Setup
    uLong compBound = compressBound(srcLen);
    uLongf compDestLen = (uLongf)compBound;
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    int rc_compress = compress(compBuf, &compDestLen, src, srcLen);

    // step 3: Core operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = (alloc_func)0;
    strm.zfree = (free_func)0;
    strm.opaque = (voidpf)0;
    unsigned char *window = new unsigned char[1];
    memset(window, 0, 1);
    int rc_inflateBackInit = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));
    unsigned long codes_used = inflateCodesUsed(&strm);

    // step 4: Validate & Cleanup
    int rc_inflateBackEnd = inflateBackEnd(&strm);
    delete [] src;
    delete [] compBuf;
    delete [] window;
    (void)version;
    (void)rc_compress;
    (void)compDestLen;
    (void)compBound;
    (void)rc_inflateBackInit;
    (void)codes_used;
    (void)rc_inflateBackEnd;
    // API sequence test completed successfully
    return 66;
}