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
//<ID> 369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload, version and buffers
    const char src[] = "zlib API sequence payload: compress2 -> crc32_z -> deflate -> gzwrite -> gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound1 = compressBound(sourceLen);
    uLongf destLen1 = (uLongf)bound1;
    Bytef *comp1 = (Bytef *)malloc((size_t)bound1);
    memset(comp1, 0, (size_t)bound1);

    // step 2: One-shot compression and compute CRC of the compressed data
    int rc_comp2 = compress2(comp1, &destLen1, source, sourceLen, 6);
    uLong crc_comp1 = crc32_z(0UL, comp1, (z_size_t)destLen1);

    // step 3: Initialize a deflate stream, deflate the compressed blob, then finalize
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = comp1;
    strm.avail_in = (uInt)destLen1;
    uLong bound2 = deflateBound(&strm, (uLong)destLen1);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound2);
    memset(comp2, 0, (size_t)bound2);
    strm.next_out = comp2;
    strm.avail_out = (uInt)bound2;
    int rc_deflate = deflate(&strm, 0);
    uLong out2 = strm.total_out;
    int rc_def_end = deflateEnd(&strm);

    // step 4: Write compressed data to a gzip file, close and cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite1 = gzwrite(gzf, comp1, (unsigned int)destLen1);
    int rc_gzwrite2 = gzwrite(gzf, comp2, (unsigned int)out2);
    int rc_gzclose = gzclose(gzf);
    free(comp1);
    free(comp2);

    (void)version;
    (void)rc_comp2;
    (void)crc_comp1;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_gzclose;
    (void)bound1;
    (void)bound2;
    (void)out2;

    // API sequence test completed successfully
    return 66;
}