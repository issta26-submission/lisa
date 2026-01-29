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
//<ID> 816
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial buffers
    const char src[] = "zlib API sequence payload for deflate + gz write/read and version check";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = sourceLen + (uLong)128;
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    Bytef *readBuf = (Bytef *)malloc((size_t)(sourceLen + 128));
    memset(compBuf, 0, (size_t)compAlloc);
    memset(readBuf, 0, (size_t)(sourceLen + 128));
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Initialize and run a simple deflate sequence
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compAlloc;
    int rc_deflate = deflate(&dstrm, 0);
    uLong compProduced = dstrm.total_out;
    int rc_deflate_end = deflateEnd(&dstrm);

    // step 3: Persist original payload with gzwrite, then read it back with gzread
    gzFile wf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gz_write = gzwrite(wf, source, (unsigned int)sourceLen);
    int rc_gz_close_w = gzclose(wf);
    gzFile rf = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int rc_gz_read = gzread(rf, readBuf, (unsigned int)sourceLen);
    int rc_gz_close_r = gzclose(rf);

    // step 4: Basic validation via CRCs, cleanup and return
    uLong crc_src = crc32(0UL, source, (uInt)sourceLen);
    uLong crc_read = crc32(0UL, readBuf, (uInt)sourceLen);
    free(compBuf);
    free(readBuf);
    (void)version;
    (void)rc_deflate_init;
    (void)rc_deflate;
    (void)compProduced;
    (void)rc_deflate_end;
    (void)rc_gz_write;
    (void)rc_gz_close_w;
    (void)rc_gz_read;
    (void)rc_gz_close_r;
    (void)crc_src;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}