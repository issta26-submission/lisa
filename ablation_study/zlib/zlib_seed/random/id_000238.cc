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
//<ID> 238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialization
    const char src_c[] = "zlib API sequence payload: compute bounds -> compress -> gzwrite/gzread -> validate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    const z_crc_t *crc_table = get_crc_table();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong dbound = deflateBound(&dstrm, sourceLen);
    uLong cbound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)cbound);
    memset(compBuf, 0, (size_t)cbound);
    uLongf compLen = (uLongf)cbound;

    // step 2: Compress the source payload using compress()
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Write compressed data to a gzip file and read it back using gzread()
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)compBuf, (unsigned int)compLen);
    int rc_gz_rewind = gzrewind(gz);
    Bytef *readBuf = (Bytef *)malloc((size_t)(compLen + 8));
    memset(readBuf, 0, (size_t)(compLen + 8));
    int rc_gz_read = gzread(gz, (voidp)readBuf, (unsigned int)compLen);

    // step 4: Validate by uncompressing the read data and cleanup
    uLongf decompLen = (uLongf)(sourceLen + 8);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, readBuf, (uLong)compLen);
    int rc_gz_close = gzclose(gz);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(readBuf);
    free(decompBuf);
    (void)compile_flags;
    (void)crc_table;
    (void)rc_def_init;
    (void)dbound;
    (void)cbound;
    (void)rc_comp;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)rc_uncomp;
    (void)rc_gz_close;
    (void)rc_def_end;
    (void)compLen;
    (void)decompLen;
    // API sequence test completed successfully
    return 66;
}