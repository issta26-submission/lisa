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
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & basic diagnostics
    const char src_c[] = "zlib API sequence payload: compute flags/crc -> bounds -> compress -> gz IO";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compile_flags = zlibCompileFlags();
    const z_crc_t *crc_table = get_crc_table();
    uLong init_crc = (uLong)(crc_table ? crc_table[0] : 1u);
    uLong crc_val = crc32(init_crc, source, (uInt)sourceLen);

    // step 2: Initialize deflate stream and compute bounds
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong def_bound = deflateBound(&dstrm, sourceLen);
    uLong comp_bound = compressBound(sourceLen);
    uLong total_bound = def_bound + comp_bound + (compile_flags & 0xFu);

    Bytef *compBuf = (Bytef *)malloc((size_t)total_bound);
    memset(compBuf, 0, (size_t)total_bound);

    // step 3: Compress the payload and exercise gz file IO (write then read)
    uLongf compLen = (uLongf)total_bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, (voidpc)source, (unsigned int)sourceLen);
    int rc_gz_rewind = gzrewind(gz);

    Bytef *readBuf = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(readBuf, 0, (size_t)(sourceLen + 64));
    int rc_gz_read = gzread(gz, (voidp)readBuf, (unsigned int)(sourceLen + 64));

    // step 4: Cleanup streams, close files and free resources
    int rc_gz_close = gzclose(gz);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(readBuf);

    (void)rc_def_init;
    (void)def_bound;
    (void)comp_bound;
    (void)total_bound;
    (void)compLen;
    (void)rc_comp;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)rc_gz_close;
    (void)rc_def_end;
    (void)compile_flags;
    (void)crc_table;
    (void)crc_val;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}