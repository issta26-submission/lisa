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
//<ID> 1251
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (source, streams, version)
    const char payload[] = "zlib_api_sequence_test_payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate, compute bounds, prepare buffers)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    uLong dbound = deflateBound(&dstrm, srcLen);
    size_t compBufSize = (size_t)(dbound + 16U);
    Bytef *compBuf = new Bytef[compBufSize];
    memset(compBuf, 0, compBufSize);
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBufSize;

    // step 3: Core operations (one-shot compress, write via gz, inflate reset-keep)
    uLong cbound = compressBound(srcLen);
    Bytef *compBuf2 = new Bytef[(size_t)cbound];
    memset(compBuf2, 0, (size_t)cbound);
    uLongf compLen_f = (uLongf)cbound;
    int rc_compress = compress(compBuf2, &compLen_f, src, srcLen);
    uLong compLen = (uLong)compLen_f;
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)compBuf2, (unsigned int)(compLen ? compLen : 0U));
    int rc_gzflush = gzflush(gzf, 0);
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf2;
    istrm.avail_in = (uInt)compLen;
    int rc_inflate_resetkeep = inflateResetKeep(&istrm);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] compBuf;
    delete [] compBuf2;
    (void)version;
    (void)rc_deflate_init;
    (void)dbound;
    (void)compBufSize;
    (void)rc_compress;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)rc_inflate_init;
    (void)rc_inflate_resetkeep;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}