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
//<ID> 455
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API fuzz payload: compress -> write gz -> inflate -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Compute CRC of source and write original source into a gzip file via gzopen64 + gzfwrite
    uLong crc_src = crc32_z(0L, source, (z_size_t)sourceLen);
    gzFile gf = gzopen64("zlib_api_sequence_out.gz", "wb");
    z_size_t gz_written = gzfwrite((voidpc)source, (z_size_t)1, (z_size_t)sourceLen, gf);
    int rc_gzclose = gzclose(gf);

    // step 3: Initialize inflate stream and perform inflate on the compressed buffer
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen + 64;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    strm.next_out = dest;
    strm.avail_out = (uInt)outCap;
    int rc_inflate = inflate(&strm, 0);
    uLong out_size = strm.total_out;

    // step 4: Validate inflate stream state, finalize and cleanup
    int rc_validate = inflateValidate(&strm, 1);
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(dest);
    (void)rc_comp;
    (void)crc_src;
    (void)gz_written;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)out_size;
    (void)rc_validate;
    (void)rc_inf_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}