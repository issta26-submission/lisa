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
//<ID> 1601
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for inflate/deflate/gz operations";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&defstrm, srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = bound;
    int rc_compress = compress2(compBuf, &compLen, src, srcLen, 6);
    int rc_deflate_end = deflateEnd(&defstrm);

    // step 2: Configure (write compressed bytes to a gz file)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, compBuf, (unsigned int)compLen);
    int rc_gzflush = gzflush(gzf, 0);
    int rc_gzclose1 = gzclose(gzf);

    // step 3: Operate (read one byte from the gz file and inflate the compressed buffer)
    gzFile gzf2 = gzopen(fname, "rb");
    int gz_first = gzgetc_(gzf2);
    z_stream instrm;
    memset(&instrm, 0, (size_t)sizeof(z_stream));
    instrm.next_in = compBuf;
    instrm.avail_in = (uInt)compLen;
    uLong outBufLen = srcLen + 64;
    Bytef *outBuf = new Bytef[(size_t)outBufLen];
    memset(outBuf, 0, (size_t)outBufLen);
    instrm.next_out = outBuf;
    instrm.avail_out = (uInt)outBufLen;
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    int rc_inflate = inflate(&instrm, 0);
    int rc_inflate_end = inflateEnd(&instrm);

    // step 4: Validate and Cleanup
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gzf2, &gz_errnum);
    int rc_gzclose2 = gzclose(gzf2);
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
    (void)rc_deflate_end;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose1;
    (void)gz_first;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)gz_errnum;
    (void)gz_errstr;
    (void)rc_gzclose2;
    // API sequence test completed successfully
    return 66;
}