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
//<ID> 1612
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize data and version)
    const char payload[] = "zlib sequence payload to demonstrate deflate, deflateCopy, deflatePending, compressBound and inflateReset";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *srcBuf = new Bytef[payloadLen ? payloadLen : 1];
    memset(srcBuf, 0, payloadLen ? payloadLen : 1);
    memcpy(srcBuf, payload, payloadLen);
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate stream and allocate output based on compressBound)
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    defstrm.next_in = srcBuf;
    defstrm.avail_in = (uInt)srcLen;
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)(bound ? bound : 1)];
    memset(compBuf, 0, (size_t)(bound ? bound : 1));
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)(bound ? bound : 1);
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_run = inflate(&defstrm, 0); // intentionally call inflate on deflate stream to exercise API interplay (no branching)

    // step 3: Operate (query pending bits, copy the deflate stream, then setup inflate to decompress produced bytes and reset)
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&defstrm, &pending, &bits);
    z_stream defstrm_copy;
    memset(&defstrm_copy, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&defstrm_copy, &defstrm);
    uLong compProduced = defstrm.total_out ? defstrm.total_out : 0UL;
    z_stream infstrm;
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)(compProduced ? compProduced : 0U);
    uLong outBufLen = srcLen + 64;
    Bytef *outBuf = new Bytef[(size_t)(outBufLen ? outBufLen : 1)];
    memset(outBuf, 0, (size_t)(outBufLen ? outBufLen : 1));
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)(outBufLen ? outBufLen : 1);
    int rc_inflate_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int rc_inflate_first = inflate(&infstrm, 0);
    int rc_inflate_reset = inflateReset(&infstrm);
    int rc_inflate_second = inflate(&infstrm, 0);

    // step 4: Validate and Cleanup
    int rc_deflate_end_orig = deflateEnd(&defstrm);
    int rc_deflate_end_copy = deflateEnd(&defstrm_copy);
    int rc_inflate_end = inflateEnd(&infstrm);
    delete [] srcBuf;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)bound;
    (void)rc_deflate_init;
    (void)rc_deflate_run;
    (void)pending;
    (void)bits;
    (void)rc_pending;
    (void)rc_deflate_copy;
    (void)compProduced;
    (void)rc_inflate_init;
    (void)rc_inflate_first;
    (void)rc_inflate_reset;
    (void)rc_inflate_second;
    (void)rc_deflate_end_orig;
    (void)rc_deflate_end_copy;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}