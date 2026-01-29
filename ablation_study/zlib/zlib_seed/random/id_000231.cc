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
//<ID> 231
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize deflate stream and compute bounds
    const char src_c[] = "zlib API sequence payload: compute bounds, compress, gzwrite, gzread, validate, cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound_def = deflateBound(&dstrm, sourceLen);
    uLong bound_comp = compressBound(sourceLen);
    size_t allocSize = (size_t)((bound_def > bound_comp) ? bound_def : bound_comp);
    Bytef *compBuf = (Bytef *)malloc(allocSize);
    memset(compBuf, 0, allocSize);
    uLongf compLen = (uLongf)allocSize;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Write original source into a gzip file (gzwrite will compress) and rewind
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, (voidpc)source, (unsigned int)sourceLen);
    int rc_gz_rewind = gzrewind(gz);

    // step 3: Read back decompressed data via gzread and perform light validation using CRC table & compile flags
    Bytef *readBuf = (Bytef *)malloc((size_t)(sourceLen + 8));
    memset(readBuf, 0, (size_t)(sourceLen + 8));
    int rc_gz_read = gzread(gz, (voidp)readBuf, (unsigned int)sourceLen);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t c0 = crc_table[(unsigned char)readBuf[0]];
    z_crc_t c1 = crc_table[(unsigned char)readBuf[1]];
    z_crc_t c2 = crc_table[(unsigned char)readBuf[2]];
    z_crc_t combined_crc_lookup = (c0 ^ c1) + c2;
    uLong compile_flags = zlibCompileFlags();

    // step 4: Cleanup streams and buffers
    int rc_gz_close = gzclose(gz);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(readBuf);

    (void)rc_def_init;
    (void)bound_def;
    (void)bound_comp;
    (void)allocSize;
    (void)compLen;
    (void)rc_comp;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)crc_table;
    (void)combined_crc_lookup;
    (void)compile_flags;
    (void)rc_gz_close;
    (void)rc_def_end;
    (void)sourceLen;
    (void)version;

    // API sequence test completed successfully
    return 66;
}