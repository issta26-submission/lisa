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
//<ID> 235
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize deflate, compute bounds and gather compile info
    const char src_c[] = "zlib API sequence payload: compute bounds -> compress -> gzwrite/gzread -> validate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compile_flags = zlibCompileFlags();
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t crc_first = crc_table[0];

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong dbound = deflateBound(&dstrm, sourceLen);
    uLong cbound = compressBound(sourceLen);
    uLong allocSize = dbound + cbound;

    Bytef *compBuf = (Bytef *)malloc((size_t)allocSize);
    memset(compBuf, 0, (size_t)allocSize);
    uLongf compLen = (uLongf)allocSize;

    // step 2: Configure & compress the source into compBuf
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Operate — write original uncompressed data to a gz file, rewind and read back
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)source, (unsigned int)sourceLen);
    int rc_gz_rewind = gzrewind(gz);
    uLong readBufLen = sourceLen + 16;
    Bytef *readBuf = (Bytef *)malloc((size_t)readBufLen);
    memset(readBuf, 0, (size_t)readBufLen);
    int rc_gz_read = gzread(gz, (voidp)readBuf, (unsigned int)sourceLen);
    int rc_gz_close = gzclose(gz);

    // step 4: Validate & Cleanup — compare read content with original, cleanup streams and buffers
    int equal = (int)(memcmp(readBuf, source, (size_t)sourceLen) == 0);
    (void)equal;
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(readBuf);
    (void)compile_flags;
    (void)crc_first;
    (void)rc_def_init;
    (void)dbound;
    (void)cbound;
    (void)allocSize;
    (void)compLen;
    (void)rc_comp2;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)rc_gz_close;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}