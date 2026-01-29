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
//<ID> 801
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate compression buffer
    const char src[] = "zlib API sequence payload for deflate, gz I/O and position queries";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);

    // step 2: Initialize deflate stream, compress the source into compBuf and finalize deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&dstrm, Z_DEFAULT_COMPRESSION, version, (int)sizeof(z_stream));
    int rc_deflate_call = deflate(&dstrm, Z_FINISH);
    uLong compProduced = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Write original data into a gz file, query positions with gztell and gzoffset64, close and reopen for reading
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    int rc_gz_flush = gzflush(gf, 0);
    off_t pos_after_write = gztell(gf);
    off64_t off64_after_write = gzoffset64(gf);
    int rc_gz_close = gzclose(gf);
    gzFile gf_read = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)sourceLen + 8);
    memset(readBuf, 0, (size_t)sourceLen + 8);
    int rc_gz_read = gzread(gf_read, readBuf, (unsigned int)sourceLen);
    off_t pos_after_read = gztell(gf_read);
    off64_t off64_after_read = gzoffset64(gf_read);
    int rc_gz_close_read = gzclose(gf_read);

    // step 4: Cleanup and quiet unused-return-value warnings
    free(compBuf);
    free(readBuf);
    (void)rc_deflate_init;
    (void)rc_deflate_call;
    (void)compProduced;
    (void)rc_deflate_end;
    (void)rc_gz_write;
    (void)rc_gz_flush;
    (void)pos_after_write;
    (void)off64_after_write;
    (void)rc_gz_close;
    (void)rc_gz_read;
    (void)pos_after_read;
    (void)off64_after_read;
    (void)rc_gz_close_read;
    (void)version;
    // API sequence test completed successfully
    return 66;
}