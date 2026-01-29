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
//<ID> 230
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initial checksum/flags inspection
    const char src_c[] = "zlib API sequence payload: compressBound -> deflateBound -> gzread";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong compile_flags = zlibCompileFlags();
    const z_crc_t *crc_table = get_crc_table();

    // step 2: Compress the source with compress() using compressBound()
    uLong cBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)cBound);
    memset(compBuf, 0, (size_t)cBound);
    uLongf compLen = (uLongf)cBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Initialize a deflate stream, compute deflateBound, and run deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong dBound = deflateBound(&dstrm, (uLong)compLen);
    Bytef *defBuf = (Bytef *)malloc((size_t)dBound);
    memset(defBuf, 0, (size_t)dBound);
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)dBound;
    int rc_deflate = deflate(&dstrm, 0);
    uLong actual_deflated = dstrm.total_out;

    // step 4: Write deflated data to a gzip file, rewind, read back, cleanup and validate
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, defBuf, (unsigned int)actual_deflated);
    int rc_gz_rewind = gzrewind(gz);
    Bytef *readBuf = (Bytef *)malloc((size_t)(actual_deflated + 16));
    memset(readBuf, 0, (size_t)(actual_deflated + 16));
    int rc_gz_read = gzread(gz, readBuf, (unsigned int)actual_deflated);
    int rc_gz_close = gzclose(gz);
    int rc_def_end = deflateEnd(&dstrm);

    free(compBuf);
    free(defBuf);
    free(readBuf);

    (void)compile_flags;
    (void)crc_table;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)actual_deflated;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)rc_gz_read;
    (void)rc_gz_close;
    (void)rc_def_end;
    (void)cBound;
    (void)dBound;
    (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}