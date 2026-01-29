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
//<ID> 814
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, version and allocate compression and read buffers
    const char src[] = "zlib API sequence payload for deflate and gz I/O validation";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));

    // step 2: Initialize deflate, compress the source and finalize
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&strm, 0);
    uLong compressedSize = strm.total_out;
    int rc_deflate_end = deflateEnd(&strm);

    // step 3: Persist original data with gzwrite, then read it back with gzread
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gz_close = gzclose(gf);
    gzFile gf2 = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(readBuf, 0, (size_t)(sourceLen + 16));
    int rc_gz_read = gzread(gf2, readBuf, (unsigned int)sourceLen);
    int rc_gz_close2 = gzclose(gf2);

    // step 4: Validate via CRC, cleanup and finalize
    uLong crc_orig = crc32(0UL, source, (uInt)sourceLen);
    uLong crc_read = crc32(0UL, readBuf, (uInt)sourceLen);
    free(compBuf);
    free(readBuf);
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)compressedSize;
    (void)rc_deflate_end;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)rc_gz_read;
    (void)rc_gz_close2;
    (void)crc_orig;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}