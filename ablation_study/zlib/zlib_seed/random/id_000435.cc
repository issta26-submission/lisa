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
//<ID> 435
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and compress into a zlib-format buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateInit2_ -> inflate -> gzprintf -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream with inflateInit2_, configure input/output, and inflate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit2_(&strm, 15, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    Bytef *dest = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(dest, 0, (size_t)(sourceLen + 16));
    strm.next_out = dest;
    strm.avail_out = (uInt)(sourceLen + 16);
    int rc_inflate = inflate(&strm, 0);
    uLong out_len = strm.total_out;

    // step 3: Use gz* API to write the inflated output via gzprintf and query compile flags
    gzFile gz = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzprintf = gzprintf(gz, "%s", (const char *)dest);
    int rc_gzclose = gzclose(gz);
    uLong compile_flags = zlibCompileFlags();

    // step 4: Cleanup inflate state and allocated buffers
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(dest);
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)out_len;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    (void)compile_flags;
    (void)version;
    // API sequence test completed successfully
    return 66;
}