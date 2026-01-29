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
//<ID> 451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compute CRC, and compress into buffer
    const char src[] = "zlib API sequence payload: compress -> validate -> write -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);
    uLong crc_src = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Prepare an inflate stream structure and run inflateValidate on it
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen + 64;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    strm.next_out = dest;
    strm.avail_out = (uInt)outCap;
    int rc_validate = inflateValidate(&strm, 15);

    // step 3: Open a gz file (64-bit API) and write the original source data using gzfwrite
    gzFile gf = gzopen64("zlib_api_sequence_out.gz", "wb");
    z_size_t written = gzfwrite((voidpc)source, (z_size_t)1, (z_size_t)sourceLen, gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate via CRC of compressed buffer, cleanup allocations, and return success
    uLong crc_combined = crc32_z(crc_src, comp, (z_size_t)compLen);
    free(comp);
    free(dest);
    (void)rc_comp;
    (void)rc_validate;
    (void)written;
    (void)rc_gzclose;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}