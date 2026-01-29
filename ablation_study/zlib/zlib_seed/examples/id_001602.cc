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
//<ID> 1602
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for inflate/deflate/gz functions";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef * src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    const char * version = zlibVersion();
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&defstrm, srcLen);
    int rc_deflate_end = deflateEnd(&defstrm);

    // step 2: Configure (compress to buffer and write gzip file)
    Bytef * compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = bound;
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);
    const char * fname = "test_zlib_api_sequence.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf_w, compBuf, (unsigned int)compLen);
    int rc_gzflush = gzflush(gzf_w, 0);
    int rc_gzclose_w = gzclose(gzf_w);

    // step 3: Operate (read a few bytes with gzgetc_ and query error state)
    gzFile gzf_r = gzopen(fname, "rb");
    int gc0 = gzgetc_(gzf_r);
    int gc1 = gzgetc_(gzf_r);
    int gc2 = gzgetc_(gzf_r);
    int gz_errnum = 0;
    const char * gz_errstr = gzerror(gzf_r, &gz_errnum);

    // step 4: Validate and Cleanup (inflate compressed buffer to output and cleanup)
    z_stream infstrm;
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    Bytef * outBuf = new Bytef[payloadLen];
    memset(outBuf, 0, payloadLen);
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)srcLen;
    int rc_inflate_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&infstrm, Z_FINISH);
    int rc_inflate_end = inflateEnd(&infstrm);
    int rc_gzclose_r = gzclose(gzf_r);

    delete [] src;
    delete [] compBuf;
    delete [] outBuf;

    (void)payloadLen;
    (void)srcLen;
    (void)rc_deflate_init;
    (void)bound;
    (void)rc_deflate_end;
    (void)rc_compress;
    (void)compLen;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose_w;
    (void)gc0;
    (void)gc1;
    (void)gc2;
    (void)gz_errnum;
    (void)gz_errstr;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzclose_r;
    (void)version;
    // API sequence test completed successfully
    return 66;
}