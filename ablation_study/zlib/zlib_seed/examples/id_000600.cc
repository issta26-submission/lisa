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
//<ID> 600
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and buffers
    const char src[] = "zlib API sequence test payload for deflateInit2_/gz* usage";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBufLen = sourceLen + 128;
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufLen);
    memset(compBuf, 0, (size_t)compBufLen);

    // step 2: Initialize and configure deflate stream using deflateInit2_
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit2_(&dstrm, 6 /*level*/, 8 /*method=Z_DEFLATED*/, 15 /*windowBits*/, 8 /*memLevel*/, 0 /*strategy*/, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBufLen;

    // step 3: Operate — run deflate once with Z_FINISH, write results to a gz file, query offset
    int rc_def = deflate(&dstrm, 4); /* 4 == Z_FINISH */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, "zlib-deflate-test\n");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)dstrm.total_out);
    off64_t file_offset = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate/cleanup — finalize deflate and free buffers
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);

    (void)rc_def_init;
    (void)rc_def;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)file_offset;
    (void)rc_gzclose;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}