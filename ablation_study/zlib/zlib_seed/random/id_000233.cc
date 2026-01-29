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
//<ID> 233
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize deflate stream and compute bounds
    const char src_c[] = "zlib API sequence payload: deflateBound & compressBound -> gzwrite/gzread -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound_def = deflateBound(&dstrm, sourceLen);

    // step 2: Determine a safe buffer size using compressBound and allocate buffers
    uLong bound_comp = compressBound(sourceLen);
    uLong totalBound = bound_def + bound_comp;
    Bytef *workBuf = (Bytef *)malloc((size_t)totalBound);
    memset(workBuf, 0, (size_t)totalBound);

    // step 3: Write the original data into a gzip file, then rewind and read it back
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "w+b");
    int rc_gz_write = gzwrite(gz, source, (unsigned int)sourceLen);
    int rc_gz_rewind = gzrewind(gz);
    uLong flags = zlibCompileFlags();
    const z_crc_t *crc_tbl = get_crc_table();
    size_t extra = (size_t)(flags & 0xFFUL);
    uLong readAlloc = sourceLen + 8 + (uLong)extra;
    Bytef *readBuf = (Bytef *)malloc((size_t)readAlloc);
    memset(readBuf, 0, (size_t)readAlloc);
    int rc_gz_read = gzread(gz, readBuf, (unsigned int)sourceLen);
    int rc_gz_close = gzclose(gz);

    // step 4: Validate usage (capture values) and cleanup resources
    int rc_def_end = deflateEnd(&dstrm);
    free(workBuf);
    free(readBuf);
    (void)rc_def_init;
    (void)bound_def;
    (void)bound_comp;
    (void)totalBound;
    (void)rc_gz_write;
    (void)rc_gz_rewind;
    (void)flags;
    (void)crc_tbl;
    (void)rc_gz_read;
    (void)rc_gz_close;
    (void)rc_def_end;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}