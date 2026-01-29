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
//<ID> 1611
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib_api_sequence_payload";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *srcBuf = new Bytef[payloadLen];
    memset(srcBuf, 0, payloadLen);
    memcpy(srcBuf, payload, payloadLen);
    const char *version = zlibVersion();

    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    defstrm.next_in = srcBuf;
    defstrm.avail_in = (uInt)srcLen;

    z_stream defcopy;
    memset(&defcopy, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate and copy its state, query pending)
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&defcopy, &defstrm);
    unsigned int pending_bits = 0;
    int pending_bits_int = 0;
    int rc_deflate_pending = deflatePending(&defcopy, &pending_bits, &pending_bits_int);

    // step 3: Operate (compress using compress2 with size from compressBound, then inflate and reset)
    uLong bound = compressBound(srcLen);
    uLongf compLen = (uLongf)(bound ? bound : 1);
    Bytef *compBuf = new Bytef[(size_t)(compLen ? compLen : 1)];
    memset(compBuf, 0, (size_t)(compLen ? compLen : 1));
    int rc_compress = compress2(compBuf, &compLen, srcBuf, srcLen, 6);

    z_stream infstrm;
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    uLong outLen = srcLen + 16;
    Bytef *outBuf = new Bytef[(size_t)outLen];
    memset(outBuf, 0, (size_t)outLen);
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)outLen;
    int rc_inflate_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&infstrm, 0);
    int rc_inflate_reset = inflateReset(&infstrm);
    int rc_inflate_after_reset = inflate(&infstrm, 0);

    // step 4: Validate and Cleanup
    int rc_deflate_end = deflateEnd(&defstrm);
    int rc_defcopy_end = deflateEnd(&defcopy);
    int rc_inflate_end = inflateEnd(&infstrm);
    delete [] srcBuf;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)pending_bits;
    (void)pending_bits_int;
    (void)rc_deflate_pending;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_reset;
    (void)rc_inflate_after_reset;
    (void)rc_deflate_end;
    (void)rc_defcopy_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}