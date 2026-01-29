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
//<ID> 606
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate buffers
    const char src[] = "zlib API sequence test payload for deflate + gz file";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBufSize = sourceLen + 128;
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);

    // step 2: Initialize and run deflate using deflateInit2_
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit2_(&strm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)sourceLen;
    strm.next_out = compBuf;
    strm.avail_out = (uInt)compBufSize;
    int rc_def = deflate(&strm, 4); /* Z_FINISH == 4 */

    // step 3: Open gz file, write compressed bytes and text via gzprintf, then query offset
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)strm.total_out);
    int rc_gzprintf = gzprintf(gf, "compressed_bytes_written\n");
    off64_t offset_after_write = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup deflate stream and free buffers
    int rc_def_end = deflateEnd(&strm);
    free(compBuf);

    (void)rc_def_init;
    (void)rc_def;
    (void)rc_gzwrite;
    (void)rc_gzprintf;
    (void)offset_after_write;
    (void)rc_gzclose;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}