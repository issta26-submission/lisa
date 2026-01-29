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
//<ID> 484
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare two input fragments, compute their adler32 checksums and combine them
    const char part1[] = "Hello ";
    const char part2[] = "zlib world!";
    const Bytef *p1 = (const Bytef *)part1;
    const Bytef *p2 = (const Bytef *)part2;
    uLong len1 = (uLong)(sizeof(part1) - 1);
    uLong len2 = (uLong)(sizeof(part2) - 1);
    uLong ad1 = adler32(0L, p1, (uInt)len1);
    uLong ad2 = adler32(0L, p2, (uInt)len2);
    uLong combined_ad = adler32_combine(ad1, ad2, (off_t)len2);
    uLong totalLen = len1 + len2;
    Bytef *source = (Bytef *)malloc((size_t)totalLen);
    memset(source, 0, (size_t)totalLen);
    memcpy(source, p1, (size_t)len1);
    memcpy(source + len1, p2, (size_t)len2);

    // step 2: Initialize and configure a deflate stream, then obtain an output bound via deflateBound
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = source;
    dstrm.avail_in = (uInt)totalLen;
    uLong outBound = deflateBound(&dstrm, (uLong)totalLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;

    // step 3: Operate deflate, write compressed output to a gzFile, query direct I/O and 64-bit offset
    int rc_def_op = deflate(&dstrm, 0);
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    int direct_flag = gzdirect(gf);
    off64_t offset64 = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup streams and buffers, and retain computed values for validation
    int rc_def_end = deflateEnd(&dstrm);
    free(source);
    free(outBuf);
    (void)ad1;
    (void)ad2;
    (void)combined_ad;
    (void)rc_def_init;
    (void)rc_def_op;
    (void)rc_gzwrite;
    (void)direct_flag;
    (void)offset64;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}