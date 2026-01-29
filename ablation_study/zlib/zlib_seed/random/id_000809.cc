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
//<ID> 809
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1
    const char src[] = "zlib API fuzz sequence: compress with deflate, write/read gz, query positions with gztell/gzoffset64";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong compAlloc = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));

    // step 2
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&dstrm, 0);
    uLong compProduced = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(gf, source, (unsigned int)sourceLen);
    off_t pos_after_write = gztell(gf);
    off64_t offset64_after_write = gzoffset64(gf);
    int rc_gz_close = gzclose(gf);
    gzFile gf2 = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(readBuf, 0, (size_t)(sourceLen + 16));
    int rc_gz_read = gzread(gf2, readBuf, (unsigned int)sourceLen);
    int rc_gz_close2 = gzclose(gf2);

    // step 4
    free(compBuf);
    free(readBuf);
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)compProduced;
    (void)rc_deflate_end;
    (void)rc_gz_write;
    (void)pos_after_write;
    (void)offset64_after_write;
    (void)rc_gz_close;
    (void)rc_gz_read;
    (void)rc_gz_close2;
    // API sequence test completed successfully
    return 66;
}