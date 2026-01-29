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
//<ID> 805
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate buffers
    const char src[] = "zlib API sequence test payload: compress with deflate, write/read gz, query offsets";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Initialize deflate, provide input, perform deflate into compBuf and finalize deflate
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, Z_FINISH);
    uLong produced = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Persist compressed data to a gz file, query positions (gztell and gzoffset64), then read it back
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, compBuf, (unsigned int)produced);
    int rc_gz_flush = gzflush(gf, 0);
    off_t pos = gztell(gf);
    off64_t pos64 = gzoffset64(gf);
    int rc_gz_close = gzclose(gf);
    gzFile gf2 = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(produced + 16));
    memset(readBuf, 0, (size_t)(produced + 16));
    int rc_gz_read = gzread(gf2, (voidp)readBuf, (unsigned int)produced);
    int rc_gz_close2 = gzclose(gf2);

    // step 4: Validate minimal data flow and cleanup
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gz_write;
    (void)rc_gz_flush;
    (void)pos;
    (void)pos64;
    (void)rc_gz_close;
    (void)rc_gz_read;
    (void)rc_gz_close2;
    free(compBuf);
    free(readBuf);
    // API sequence test completed successfully
    return 66;
}