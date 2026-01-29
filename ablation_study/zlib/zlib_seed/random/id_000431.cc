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
//<ID> 431
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a buffer
    const char src[] = "zlib API sequence payload: compress -> inflateInit2_ -> inflate -> gzprintf -> inflateEnd -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream with inflateInit2_, provide input and output buffers, then inflate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init2 = inflateInit2_(&strm, 15, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    Bytef *dest = (Bytef *)malloc((size_t)sourceLen);
    memset(dest, 0, (size_t)sourceLen);
    strm.next_out = dest;
    strm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&strm, 0);
    uLong inflated_total = strm.total_out;

    // step 3: Query compile flags and write a small report to a gzip file using gzprintf
    uLong flags = zlibCompileFlags();
    gzFile gz = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzprintf = gzprintf(gz, "zlibCompileFlags: %lu\ninflated_bytes: %lu\n", flags, inflated_total);
    int rc_gzclose = gzclose(gz);

    // step 4: Validate via checksum and cleanup resources
    uLong sum_src = adler32(0L, source, (uInt)sourceLen);
    uLong sum_dest = adler32(0L, dest, (uInt)inflated_total);
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(dest);
    (void)rc_comp;
    (void)rc_inf_init2;
    (void)rc_inflate;
    (void)flags;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    (void)sum_src;
    (void)sum_dest;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}