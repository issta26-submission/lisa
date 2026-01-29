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
//<ID> 1613
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for compress/deflate/inflate copy and pending checks";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *srcBuf = new Bytef[payloadLen];
    memset(srcBuf, 0, payloadLen);
    memcpy(srcBuf, payload, payloadLen);
    const char *version = zlibVersion();

    // step 2: Setup (compress source to produce compressed data and initialize deflate stream)
    uLong bound = compressBound(srcLen);
    uLongf compLen = bound ? bound : 1;
    Bytef *compBuf = new Bytef[(size_t)compLen];
    memset(compBuf, 0, (size_t)compLen);
    int rc_compress = compress2(compBuf, &compLen, srcBuf, srcLen, 6);

    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    defstrm.next_in = srcBuf;
    defstrm.avail_in = (uInt)srcLen;
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    unsigned int pending = 0;
    int bits = 0;
    int rc_deflate_pending = deflatePending(&defstrm, &pending, &bits);

    z_stream defcopy;
    memset(&defcopy, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&defcopy, &defstrm);

    // step 3: Operate (inflate compressed data, reset inflate stream and inflate again)
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
    int rc_inflate_first = inflate(&infstrm, 0);
    int rc_inflate_reset = inflateReset(&infstrm);
    int rc_inflate_second = inflate(&infstrm, 0);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&infstrm);
    int rc_deflate_end_src = deflateEnd(&defstrm);
    int rc_deflate_end_copy = deflateEnd(&defcopy);
    delete [] srcBuf;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_deflate_pending;
    (void)pending;
    (void)bits;
    (void)rc_deflate_copy;
    (void)rc_inflate_init;
    (void)rc_inflate_first;
    (void)rc_inflate_reset;
    (void)rc_inflate_second;
    (void)rc_inflate_end;
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_copy;
    // API sequence test completed successfully
    return 66;
}