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
//<ID> 609
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and output buffer
    const char src[] = "zlib API sequence test payload";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uInt compBufSize = (uInt)(sourceLen + 128);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);

    // step 2: Initialize and run deflate with custom parameters (deflateInit2_)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init2 = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = compBufSize;
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Open a gzFile, write a header with gzprintf, write compressed bytes, and query offset with gzoffset64
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, "Compressed payload follows\n");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)dstrm.total_out);
    off64_t offset_after_write = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup buffers and silence unused-return warnings
    free(compBuf);
    (void)rc_def_init2;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)offset_after_write;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}