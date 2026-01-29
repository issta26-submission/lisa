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
//<ID> 811
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and buffers
    const char src[] = "zlib API sequence payload for deflateInit_, gzwrite and gzread";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = sourceLen + 64UL;
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    Bytef *readBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(readBuf, 0, (size_t)(sourceLen + 16));

    // step 2: Initialize z_stream and perform a deflate pass (demonstration)
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&strm, 0);
    uLong compProduced = strm.total_out;
    int rc_deflate_end = deflateEnd(&strm);

    // step 3: Write the original source to a gzip file and close
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gz_close = gzclose(gf);

    // step 4: Read back from the gzip file into readBuf, cleanup resources
    gzFile gf2 = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gz_read = gzread(gf2, readBuf, (unsigned int)sourceLen);
    int rc_gz_close2 = gzclose(gf2);
    free(compBuf);
    free(readBuf);
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)compProduced;
    (void)rc_deflate_end;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)rc_gz_read;
    (void)rc_gz_close2;
    // API sequence test completed successfully
    return 66;
}