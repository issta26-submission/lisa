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
//<ID> 1605
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for inflate/deflate/gz APIs";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    defstrm.next_in = src;
    defstrm.avail_in = (uInt)srcLen;
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&defstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = bound;
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);

    // step 2: Configure (write compressed bytes to a gz file)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf_w, compBuf, (unsigned int)compLen);
    int rc_gzflush = gzflush(gzf_w, 0);
    int rc_gzclose_w = gzclose_w(gzf_w);

    // step 3: Operate (read a byte via gzgetc_ and inspect gzerror; prepare inflate)
    gzFile gzf_r = gzopen(fname, "rb");
    int c = gzgetc_(gzf_r);
    int errnum = 0;
    const char *gerr = gzerror(gzf_r, &errnum);
    z_stream infstrm;
    memset(&infstrm, 0, (size_t)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    Bytef *outBuf = new Bytef[payloadLen + 16];
    memset(outBuf, 0, payloadLen + 16);
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)(payloadLen + 16);
    int rc_inflate_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&infstrm, 0);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&infstrm);
    int rc_gzclose_r = gzclose_r(gzf_r);
    int rc_deflate_end = deflateEnd(&defstrm);
    delete [] src;
    delete [] compBuf;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)rc_deflate_init;
    (void)bound;
    (void)compLen;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose_w;
    (void)c;
    (void)errnum;
    (void)gerr;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzclose_r;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}