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
//<ID> 808
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and stream
    const char src[] = "zlib API sequence payload to compress, write to gz, read back and inspect offsets";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Initialize deflate, allocate output buffer using compressBound, compress data
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    uLong outAlloc = compressBound(sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    strm.next_out = outBuf;
    strm.avail_out = (uInt)outAlloc;
    int rc_deflate = deflate(&strm, 4); /* Z_FINISH */
    uLong produced = strm.total_out;
    int rc_deflate_end = deflateEnd(&strm);

    // step 3: Persist compressed data to a gz file, obtain positions (gztell and gzoffset64), then read back
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, outBuf, (unsigned int)produced);
    off_t pos = gztell(gf);
    off64_t pos64 = gzoffset64(gf);
    int rc_gz_close_w = gzclose(gf);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *inBuf = (Bytef *)malloc((size_t)(produced + 16));
    memset(inBuf, 0, (size_t)(produced + 16));
    int rc_gz_read = gzread(gr, inBuf, (unsigned int)produced);
    int rc_gz_close_r = gzclose(gr);

    // step 4: Cleanup and suppress unused warnings
    free(outBuf);
    free(inBuf);
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)rc_deflate_end;
    (void)rc_gz_write;
    (void)pos;
    (void)pos64;
    (void)rc_gz_close_w;
    (void)rc_gz_read;
    (void)rc_gz_close_r;
    (void)version;
    (void)sourceLen;
    (void)produced;
    // API sequence test completed successfully
    return 66;
}