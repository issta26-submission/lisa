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
//<ID> 1272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for testing";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);
    uLong compLen = (uLong)compLen_f;

    z_stream defstrm;
    z_stream defCopy;
    z_stream instrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    memset(&defCopy, 0, (size_t)sizeof(z_stream));
    memset(&instrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate/inflate streams and prepare output buffer)
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    Bytef *defOut = new Bytef[(size_t)(compLen + 16UL)];
    memset(defOut, 0, (size_t)(compLen + 16UL));
    defstrm.next_in = compBuf;
    defstrm.avail_in = (uInt)compLen;
    defstrm.next_out = defOut;
    defstrm.avail_out = (uInt)(compLen + 16UL);

    int rc_deflate_copy = deflateCopy(&defCopy, &defstrm);
    defCopy.next_out = defOut;
    defCopy.avail_out = (uInt)(compLen + 16UL);

    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    instrm.next_in = compBuf;
    instrm.avail_in = (uInt)compLen;

    // step 3: Core operations (validate inflate state, write gz file and seek, inspect compile flags)
    int rc_inflate_validate = inflateValidate(&instrm, 1);
    gzFile gzf = gzopen("zlib_seq_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)compBuf, (unsigned int)compLen);
    off_t newpos = gzseek(gzf, (off_t)0, 0);
    uLong compile_flags = zlibCompileFlags();

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end_orig = deflateEnd(&defstrm);
    int rc_deflate_end_copy = deflateEnd(&defCopy);
    int rc_inflate_end = inflateEnd(&instrm);
    delete [] src;
    delete [] compBuf;
    delete [] defOut;
    (void)version;
    (void)bound;
    (void)rc_compress;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)rc_gzwrite;
    (void)newpos;
    (void)compile_flags;
    (void)rc_gzclose;
    (void)rc_deflate_end_orig;
    (void)rc_deflate_end_copy;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}