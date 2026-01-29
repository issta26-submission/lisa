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
//<ID> 1279
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for deflateCopy and gzseek";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;

    // step 2: Setup (produce a compressed buffer and prepare deflate streams)
    int rc_compress = compress(compBuf, &compLen_f, src, srcLen);
    uLong compLen = (uLong)compLen_f;
    z_stream defsrc;
    z_stream defcopy;
    memset(&defsrc, 0, (size_t)sizeof(z_stream));
    memset(&defcopy, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&defsrc, 6, version, (int)sizeof(z_stream));
    defsrc.next_in = compBuf;
    defsrc.avail_in = (uInt)compLen;
    Bytef *defOut = new Bytef[(size_t)(compLen + 16UL)];
    memset(defOut, 0, (size_t)(compLen + 16UL));
    defsrc.next_out = defOut;
    defsrc.avail_out = (uInt)(compLen + 16UL);
    int rc_deflate_copy = deflateCopy(&defcopy, &defsrc);

    // step 3: Operate and Validate (write compressed data to gzip file, seek, and validate inflate stream)
    gzFile gzf = gzopen("zlib_seq_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);
    off_t pos_after_write = gzseek(gzf, 0, 0);
    z_stream instrm;
    memset(&instrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    int rc_inflate_validate = inflateValidate(&instrm, 1);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end_src = deflateEnd(&defsrc);
    int rc_deflate_end_copy = deflateEnd(&defcopy);
    int rc_inflate_end = inflateEnd(&instrm);
    delete [] src;
    delete [] compBuf;
    delete [] defOut;
    (void)version;
    (void)compile_flags;
    (void)rc_compress;
    (void)bound;
    (void)compLen_f;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)rc_gzclose;
    (void)rc_deflate_end_src;
    (void)rc_deflate_end_copy;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}