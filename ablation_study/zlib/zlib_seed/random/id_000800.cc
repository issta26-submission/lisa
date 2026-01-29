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
//<ID> 800
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, compute bounds and initialize deflate stream
    const char src[] = "zlib API sequence payload to compress, write to gz, then read and inspect offsets";
    const Bytef *input = (const Bytef *)src;
    uLong inputLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;
    uLong outAlloc = compressBound(inputLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outAlloc);
    memset(outBuf, 0, (size_t)outAlloc);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outAlloc;

    // step 2: Compress with deflate (single-shot) and finalize stream
    int rc_deflate = deflate(&dstrm, 4); /* 4 == Z_FINISH */
    uLong compressedProduced = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Write original data to a gzip file, then reopen and read it back; get offsets
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gw, src, (unsigned int)inputLen);
    int rc_gz_close_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    voidp readBuf = malloc((size_t)(inputLen + 16));
    memset(readBuf, 0, (size_t)(inputLen + 16));
    int rc_gz_read = gzread(gr, readBuf, (unsigned int)(inputLen + 16));
    off_t pos = gztell(gr);
    off64_t pos64 = gzoffset64(gr);
    int rc_gz_close_r = gzclose(gr);

    // step 4: Cleanup and finalize
    free(outBuf);
    free(readBuf);
    (void)rc_def_init;
    (void)rc_deflate;
    (void)compressedProduced;
    (void)rc_def_end;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_gz_read;
    (void)pos;
    (void)pos64;
    (void)rc_gz_close_r;
    // API sequence test completed successfully
    return 66;
}