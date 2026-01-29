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
//<ID> 1619
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for compress and inflate";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)(bound ? bound : 1)];
    memset(compBuf, 0, (size_t)(bound ? bound : 1));
    uLongf compLen = bound ? bound : 1;
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);

    // step 2: Setup (deflate streams and copy)
    z_stream defA;
    memset(&defA, 0, (size_t)sizeof(z_stream));
    defA.next_in = src;
    defA.avail_in = (uInt)srcLen;
    defA.next_out = compBuf;
    defA.avail_out = (uInt)(compLen ? compLen : 1);
    int rc_defA_init = deflateInit_(&defA, 6, version, (int)sizeof(z_stream));
    z_stream defB;
    memset(&defB, 0, (size_t)sizeof(z_stream));
    int rc_defcopy = deflateCopy(&defB, &defA);
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&defA, &pending, &bits);

    // step 3: Core operations (inflate, reset, inflate again)
    z_stream inf;
    memset(&inf, 0, (size_t)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    uLong outLen = srcLen + 32;
    Bytef *outBuf = new Bytef[(size_t)outLen];
    memset(outBuf, 0, (size_t)outLen);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)outLen;
    int rc_inflate_init = inflateInit_(&inf, version, (int)sizeof(z_stream));
    int rc_inflate_first = inflate(&inf, 0);
    int rc_inflate_reset = inflateReset(&inf);
    int rc_inflate_second = inflate(&inf, 0);
    int rc_inflate_end = inflateEnd(&inf);

    // step 4: Validate and Cleanup
    int rc_defA_end = deflateEnd(&defA);
    int rc_defB_end = deflateEnd(&defB);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)rc_defA_init;
    (void)rc_defcopy;
    (void)pending;
    (void)bits;
    (void)rc_pending;
    (void)rc_inflate_init;
    (void)rc_inflate_first;
    (void)rc_inflate_reset;
    (void)rc_inflate_second;
    (void)rc_inflate_end;
    (void)rc_defA_end;
    (void)rc_defB_end;
    // API sequence test completed successfully
    return 66;
}