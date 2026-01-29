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
//<ID> 605
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and allocate compression buffer
    const char src[] = "zlib API sequence: payload for deflate and gz operations";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Initialize and configure deflate stream using deflateInit2_
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBound;

    // step 3: Operate: perform deflate, finalize stream, write metadata to gzFile and query offset
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, "compressed_bytes=%lu\n", (unsigned long)dstrm.total_out);
    off64_t off = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup and finalize validation
    free(compBuf);
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_gzprintf;
    (void)off;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}