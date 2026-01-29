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
//<ID> 458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateInit2_ -> inflateValidate -> inflate -> write gz64 -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream, set inputs/outputs and call inflateValidate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit2_(&strm, 15, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen * 2 + 16;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    strm.next_out = dest;
    strm.avail_out = (uInt)outCap;
    int rc_inf_validate = inflateValidate(&strm, 15);

    // step 3: Perform inflate to decompress, compute crc32_z checksums, and write output via gzopen64/gzfwrite
    int rc_inflate = inflate(&strm, 0);
    uLong out_size = strm.total_out;
    uLong crc_src = crc32_z(0L, source, (z_size_t)sourceLen);
    uLong crc_out = crc32_z(0L, dest, (z_size_t)out_size);
    gzFile gf = gzopen64("zlib_api_sequence_out64.gz", "wb");
    z_size_t written = gzfwrite((voidpc)dest, (z_size_t)1, (z_size_t)out_size, gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(dest);
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inf_validate;
    (void)rc_inflate;
    (void)crc_src;
    (void)crc_out;
    (void)written;
    (void)rc_gzclose;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}