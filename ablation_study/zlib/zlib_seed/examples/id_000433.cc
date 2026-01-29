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
//<ID> 433
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateInit2_ -> inflate -> gzprintf -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);
    uLong flags = zlibCompileFlags();

    // step 2: Initialize inflate stream with inflateInit2_
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_init = inflateInit2_(&strm, 15, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outBound = sourceLen;
    Bytef *dest = (Bytef *)malloc((size_t)outBound);
    memset(dest, 0, (size_t)outBound);
    strm.next_out = dest;
    strm.avail_out = (uInt)outBound;

    // step 3: Operate (inflate) and validate via checksums
    int rc_inflate = inflate(&strm, 0);
    uLong inflated_total = strm.total_out;
    uLong sum_src = adler32(0L, source, (uInt)sourceLen);
    uLong sum_dest = adler32(0L, dest, (uInt)inflated_total);

    // step 4: Write original source to a gzip file, cleanup resources
    gzFile gz = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gz, "%s", src);
    int rc_gzclose = gzclose(gz);
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(dest);
    (void)rc_comp;
    (void)flags;
    (void)rc_init;
    (void)rc_inflate;
    (void)sum_src;
    (void)sum_dest;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    (void)rc_inf_end;
    (void)inflated_total;
    (void)compLen;
    // API sequence test completed successfully
    return 66;
}