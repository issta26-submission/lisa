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
//<ID> 485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare two short source blocks, compute individual adler32 checksums and combine them
    const char part1[] = "part_one_";
    const char part2[] = "part_two";
    const Bytef *p1 = (const Bytef *)part1;
    const Bytef *p2 = (const Bytef *)part2;
    uLong len1 = (uLong)(sizeof(part1) - 1);
    uLong len2 = (uLong)(sizeof(part2) - 1);
    uLong ad1 = adler32(1L, p1, (uInt)len1);
    uLong ad2 = adler32(1L, p2, (uInt)len2);
    off_t offlen2 = (off_t)len2;
    uLong combined_ad = adler32_combine(ad1, ad2, offlen2);

    // step 2: Concatenate sources, compute compress bound and perform a single-pass compress
    uLong srcLen = len1 + len2;
    Bytef *concat = (Bytef *)malloc((size_t)srcLen);
    memcpy(concat, p1, (size_t)len1);
    memcpy(concat + len1, p2, (size_t)len2);
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(compBuf, &compLen, concat, srcLen, 6);

    // step 3: Initialize a deflate stream, ask for deflateBound, run deflate to produce output buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *ver = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = compBuf;
    dstrm.avail_in = (uInt)compLen;
    uLong outBound = deflateBound(&dstrm, (uLong)compLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def = deflate(&dstrm, 0);

    // step 4: Write deflate output to a gzFile, query direct I/O and 64-bit offset, then cleanup all resources
    gzFile gf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    int is_direct = gzdirect(gf);
    off64_t pos64 = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);
    int rc_def_end = deflateEnd(&dstrm);
    free(concat);
    free(compBuf);
    free(outBuf);
    (void)ad1;
    (void)ad2;
    (void)combined_ad;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_gzwrite;
    (void)is_direct;
    (void)pos64;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}