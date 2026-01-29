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
//<ID> 482
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute partial adler32 checksums
    const char src[] = "zlib fuzz driver sequence sample data";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uInt part1 = (uInt)(sourceLen / 2);
    uInt part2 = (uInt)(sourceLen - part1);
    uLong ad1 = adler32(1L, source, part1);
    uLong ad2 = adler32(1L, source + part1, part2);

    // step 2: Initialize deflate stream, set input, and compute/allocate output bound
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def = deflate(&dstrm, 0);

    // step 3: Write deflate output to a gzFile and query direct I/O and 64-bit offset
    gzFile gf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (const void *)outBuf, (unsigned int)dstrm.total_out);
    int direct = gzdirect(gf);
    off64_t off64 = gzoffset64(gf);

    // step 4: Combine adler32 values, cleanup streams and memory, finalize
    uLong combined_adler = adler32_combine(ad1, ad2, (off_t)part2);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    free(outBuf);
    (void)version;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_gzwrite;
    (void)direct;
    (void)off64;
    (void)combined_adler;
    (void)rc_def_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}