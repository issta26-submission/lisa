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
//<ID> 459
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateInit_ -> inflateValidate -> inflate -> crc32_z -> gzopen64/gzfwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and configure for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen * 2;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    strm.next_out = dest;
    strm.avail_out = (uInt)outCap;

    // step 3: Validate stream and perform inflate, then compute CRC and write output via gzopen64/gzfwrite
    int rc_validate = inflateValidate(&strm, 1);
    int rc_inflate = inflate(&strm, 0);
    uLong out_size = strm.total_out;
    int rc_inf_end = inflateEnd(&strm);
    uLong crc_out = crc32_z(0UL, dest, (z_size_t)out_size);
    gzFile gf = gzopen64("zlib_api_sequence_out.gz", "wb");
    z_size_t written = gzfwrite((voidpc)dest, (z_size_t)1, (z_size_t)out_size, gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup
    free(comp);
    free(dest);
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_validate;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)crc_out;
    (void)written;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}