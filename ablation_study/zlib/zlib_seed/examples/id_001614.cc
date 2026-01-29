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
//<ID> 1614
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib sequence payload to test compressBound, deflateCopy, deflatePending, inflate and inflateReset";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const char *version = zlibVersion();

    // compute an upper bound for compressed data and allocate
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)(bound ? bound : 1)];
    memset(compBuf, 0, (size_t)(bound ? bound : 1));
    uLongf compLen = bound ? bound : 1;
    int rc_compress2 = compress2(compBuf, &compLen, src, srcLen, 6);

    // step 2: Setup (initialize deflate stream and make a copy)
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    z_stream defcopy;
    memset(&defcopy, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&defcopy, &defstrm);
    unsigned int pending = 0;
    int bits = 0;
    int rc_deflate_pending = deflatePending(&defstrm, &pending, &bits);

    // step 3: Core operations (inflate the compressed data and reset inflate state)
    z_stream infstrm;
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    uLong outBufLen = srcLen + 32;
    Bytef *outBuf = new Bytef[(size_t)outBufLen];
    memset(outBuf, 0, (size_t)outBufLen);
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)outBufLen;
    int rc_inflate_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&infstrm, 0);
    int rc_inflate_reset = inflateReset(&infstrm);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&infstrm);
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_defcopy_end = deflateEnd(&defcopy);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)bound;
    (void)compLen;
    (void)rc_compress2;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)pending;
    (void)bits;
    (void)rc_deflate_pending;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_reset;
    (void)rc_inflate_end;
    (void)rc_deflate_end;
    (void)rc_defcopy_end;
    // API sequence test completed successfully
    return 66;
}