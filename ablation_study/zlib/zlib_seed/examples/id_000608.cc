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
//<ID> 608
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and buffers
    const char src[] = "zlib API sequence test payload for deflateInit2_, gzopen, gzprintf, gzoffset64";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Initialize and configure deflate using deflateInit2_
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));

    // step 3: Perform compression, write compressed bytes to a gzFile, and query offset
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    uLong compSize = dstrm.total_out;
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compSize);
    int rc_gzprintf = gzprintf(gf, "zlib test: wrote compressed payload\n");
    off64_t offset_after_write = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup deflate stream and free buffers
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    (void)rc_def_init;
    (void)rc_def;
    (void)compSize;
    (void)rc_gzwrite;
    (void)rc_gzprintf;
    (void)offset_after_write;
    (void)rc_gzclose;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}