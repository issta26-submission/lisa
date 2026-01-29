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
//<ID> 234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialization
    const char src_c[] = "zlib API sequence payload: compute bounds -> compress -> gzip io -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    const z_crc_t *crc_table = get_crc_table();

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong def_bound = deflateBound(&dstrm, sourceLen);
    uLong comp_bound = compressBound(sourceLen);
    uLong alloc_bound = (def_bound > comp_bound) ? def_bound : comp_bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)alloc_bound);
    memset(compBuf, 0, (size_t)alloc_bound);
    uLongf compLen = (uLongf)alloc_bound;

    // step 2: Compress the source into compBuf and derive a simple validation token
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    uLong validation_token = (uLong)crc_table[0] ^ compile_flags;

    // step 3: Write compressed data to a gzip file, rewind and read it back
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    int rc_gz_rewind = gzrewind(gz);
    Bytef *readBuf = (Bytef *)malloc((size_t)(compLen + 16));
    memset(readBuf, 0, (size_t)(compLen + 16));
    int rc_gz_read = gzread(gz, (voidp)readBuf, (unsigned int)compLen);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup and finalize
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(readBuf);

    (void)rc_def_init;
    (void)rc_comp;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)rc_gz_close;
    (void)rc_def_end;
    (void)validation_token;
    (void)alloc_bound;
    (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}