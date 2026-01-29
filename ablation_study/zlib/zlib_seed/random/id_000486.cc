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
//<ID> 486
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare two input parts, compute individual adlers, combine them, and build the full source buffer
    const char part1[] = "zlib part one: hello ";
    const char part2[] = "and zlib part two: world!";
    const Bytef *p1 = (const Bytef *)part1;
    const Bytef *p2 = (const Bytef *)part2;
    uLong len1 = (uLong)(sizeof(part1) - 1);
    uLong len2 = (uLong)(sizeof(part2) - 1);
    uLong a1 = adler32(0UL, p1, (uInt)len1);
    uLong a2 = adler32(0UL, p2, (uInt)len2);
    uLong a_combined = adler32_combine(a1, a2, (off_t)len2);
    uLong srcLen = len1 + len2;
    Bytef *src = (Bytef *)malloc((size_t)srcLen);
    memcpy(src, p1, (size_t)len1);
    memcpy(src + len1, p2, (size_t)len2);

    // step 2: Initialize a deflate stream, set input, and query deflateBound for output buffer sizing
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    uLong outBound = deflateBound(&dstrm, srcLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;

    // step 3: Operate deflate, write output to a gzFile, call gzdirect and gzoffset64, and combine adler with stream adler
    int rc_def = deflate(&dstrm, 0);
    gzFile gf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    int direct = gzdirect(gf);
    off64_t off64 = gzoffset64(gf);
    uLong final_adler = adler32_combine(a_combined, dstrm.adler, (off_t)srcLen);

    // step 4: Cleanup streams, file, and memory
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    free(src);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_gzwrite;
    (void)direct;
    (void)off64;
    (void)final_adler;
    (void)rc_def_end;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}