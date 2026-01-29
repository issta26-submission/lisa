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
//<ID> 1618
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence: deflate copy, pending, compressBound, inflate and reset";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen ? payloadLen : 1];
    memset(src, 0, payloadLen ? payloadLen : 1);
    memcpy(src, payload, payloadLen);
    const char *version = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (bounds, copy and pending query)
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)(bound ? bound : 1)];
    memset(compBuf, 0, (size_t)(bound ? bound : 1));
    z_stream defstrm_copy;
    memset(&defstrm_copy, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&defstrm_copy, &defstrm);
    unsigned int pending = 0;
    int bits = 0;
    int rc_deflate_pending = deflatePending(&defstrm, &pending, &bits);

    // step 3: Operate (compress with compress2, then inflate and reset)
    uLongf compLen = (uLongf)(bound ? bound : 1);
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);
    z_stream infstrm;
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)(compLen ? compLen : 0);
    uLong outBufLen = srcLen + 64;
    Bytef *outBuf = new Bytef[(size_t)(outBufLen ? outBufLen : 1)];
    memset(outBuf, 0, (size_t)(outBufLen ? outBufLen : 1));
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)(outBufLen ? outBufLen : 0);
    int rc_inflate_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&infstrm, 0);
    int rc_inflate_reset = inflateReset(&infstrm);

    // step 4: Validate and Cleanup
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_deflate_copy_end = deflateEnd(&defstrm_copy);
    int rc_inflate_end = inflateEnd(&infstrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)rc_deflate_init;
    (void)bound;
    (void)rc_deflate_copy;
    (void)pending;
    (void)bits;
    (void)rc_deflate_pending;
    (void)compLen;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_reset;
    (void)rc_deflate_end;
    (void)rc_deflate_copy_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}