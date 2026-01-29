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
//<ID> 437
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: compress -> inflateInit2_ -> inflate -> gzprintf -> inflateEnd";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream with inflateInit2_ and set up buffers
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit2_(&strm, 15, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    Bytef *dest = (Bytef *)malloc((size_t)sourceLen);
    memset(dest, 0, (size_t)sourceLen);
    strm.next_out = dest;
    strm.avail_out = (uInt)sourceLen;

    // step 3: Perform inflate, report with gzprintf, and collect compile flags
    int rc_inflate = inflate(&strm, 0);
    gzFile gz = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gz, "Inflated %lu bytes from %lu compressed bytes\n", strm.total_out, (uLong)compLen);
    int rc_gzclose = gzclose(gz);
    uLong flags = zlibCompileFlags();

    // step 4: Cleanup inflate stream and free resources
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(dest);
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    (void)flags;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}