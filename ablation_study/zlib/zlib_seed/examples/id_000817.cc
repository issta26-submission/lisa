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
//<ID> 817
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and allocations
    const char src[] = "zlib API sequence payload: deflate init, compress, gz write/read and cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));

    // step 2: Initialize deflate and perform a single deflate call
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&strm, 0);
    uLong compProduced = strm.total_out;
    int rc_deflate_end = deflateEnd(&strm);

    // step 3: Persist original data with gzwrite, then read it back with gzread
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gw, source, (unsigned int)sourceLen);
    int rc_gz_close_w = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(readBuf, 0, (size_t)(sourceLen + 16));
    int rc_gz_read = gzread(gr, readBuf, (unsigned int)sourceLen);
    int rc_gz_close_r = gzclose(gr);

    // step 4: Cleanup and lightweight validation (capture values)
    free(compBuf);
    free(readBuf);
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)compProduced;
    (void)rc_deflate_end;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_gz_read;
    (void)rc_gz_close_r;
    (void)version;
    // API sequence test completed successfully
    return 66;
}