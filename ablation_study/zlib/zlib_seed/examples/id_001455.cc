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
//<ID> 1455
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
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Setup
    uLongf compDestLen = (uLongf)compBound;
    int rc_compress = compress(compBuf, &compDestLen, src, srcLen);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    const int windowBits = 15;
    unsigned char *window = new unsigned char[(size_t)1 << windowBits];
    memset(window, 0, (size_t)1 << windowBits);
    int rc_inflate_init = inflateBackInit_(&strm, windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Core operations
    unsigned long codes_used_before = inflateCodesUsed(&strm);
    (void)codes_used_before;
    (void)rc_compress;
    (void)compDestLen;
    (void)compBound;

    // step 4: Cleanup
    int rc_inflate_end = inflateBackEnd(&strm);
    delete [] src;
    delete [] compBuf;
    delete [] window;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}