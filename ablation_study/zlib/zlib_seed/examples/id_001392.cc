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
//<ID> 1392
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for inflateValidate/deflate/gzdopen/compress/gzoffset64";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    Bytef *deflateOut = new Bytef[(size_t)compBound];
    memset(deflateOut, 0, (size_t)compBound);
    Bytef *reconBuf = new Bytef[(size_t)srcLen];
    memset(reconBuf, 0, (size_t)srcLen);

    // step 2: Setup (compress source using high-level API)
    uLongf compLen = (uLongf)compBound;
    int rc_compress = compress(compBuf, &compLen, src, srcLen);

    // step 3: Core operations (initialize deflate, run deflate on compressed data, then validate and inflate)
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    dstrm.next_out = deflateOut;
    dstrm.avail_out = (uInt)compBound;
    int rc_deflate = deflate(&dstrm, 0);
    uLong d_out_len = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = deflateOut;
    istrm.avail_in = (uInt)d_out_len;
    istrm.next_out = reconBuf;
    istrm.avail_out = (uInt)srcLen;
    int rc_inflate_validate = inflateValidate(&istrm, 1);
    int rc_inflate = inflate(&istrm, 0);
    uLong reconLen = istrm.total_out;
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Cleanup (write original data to a gz stream opened from stdout and query its offset)
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, src, (unsigned int)srcLen);
    off64_t gzf_offset = gzoffset64(gzf);
    int rc_gzclose = gzclose(gzf);

    delete [] src;
    delete [] compBuf;
    delete [] deflateOut;
    delete [] reconBuf;
    (void)version;
    (void)compBound;
    (void)compLen;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)d_out_len;
    (void)rc_deflate_end;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)rc_inflate;
    (void)reconLen;
    (void)rc_inflate_end;
    (void)gzf;
    (void)rc_gzwrite;
    (void)gzf_offset;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}