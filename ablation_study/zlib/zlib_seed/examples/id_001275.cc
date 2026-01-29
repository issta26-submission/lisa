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
//<ID> 1275
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Zlib API sequence payload for deflateCopy/gzseek/inflateValidate";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    int rc_compress = compress(compBuf, &compLen_f, src, srcLen);
    uLong compLen = (uLong)compLen_f;
    z_stream defstrm;
    z_stream defcopy;
    z_stream instrm;
    memset(&defstrm, 0, (size_t)sizeof(z_stream));
    memset(&defcopy, 0, (size_t)sizeof(z_stream));
    memset(&instrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate stream and prepare output)
    int rc_deflate_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    Bytef *defOut = new Bytef[(size_t)(compLen + 64UL)];
    memset(defOut, 0, (size_t)(compLen + 64UL));
    defstrm.next_in = compBuf;
    defstrm.avail_in = (uInt)compLen;
    defstrm.next_out = defOut;
    defstrm.avail_out = (uInt)(compLen + 64UL);

    // step 3: Core operations (copy deflate state, gzip I/O, validate inflate, inspect compile flags)
    int rc_deflate_copy = deflateCopy(&defcopy, &defstrm);
    uLong compile_flags = zlibCompileFlags();
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidp)compBuf, (unsigned int)compLen);
    off_t rc_gzseek = gzseek(gzf, (off_t)0, 0);
    int rc_inflate_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    instrm.next_in = compBuf;
    instrm.avail_in = (uInt)compLen;
    int rc_inflate_validate = inflateValidate(&instrm, 0);

    // step 4: Cleanup and finalization
    int rc_deflate_end1 = deflateEnd(&defstrm);
    int rc_deflate_end2 = deflateEnd(&defcopy);
    int rc_inflate_end = inflateEnd(&instrm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    delete [] defOut;
    (void)version;
    (void)rc_compress;
    (void)bound;
    (void)compLen_f;
    (void)rc_deflate_init;
    (void)rc_deflate_copy;
    (void)compile_flags;
    (void)rc_gzwrite;
    (void)rc_gzseek;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)rc_deflate_end1;
    (void)rc_deflate_end2;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}