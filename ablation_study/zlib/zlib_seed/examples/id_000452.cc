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
//<ID> 452
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> validate -> write gz64 -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize an inflate stream structure and run inflateValidate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen * 2 + 16;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    strm.next_out = dest;
    strm.avail_out = (uInt)outCap;
    int rc_validate = inflateValidate(&strm, 1);

    // step 3: Compute checksum of the compressed data and write it to a gz stream opened with gzopen64
    uLong crc_comp = crc32_z(0L, comp, (z_size_t)compLen);
    gzFile gf = gzopen64("zlib_api_sequence_out.gz", "wb");
    z_size_t written = gzfwrite((voidpc)comp, 1, (z_size_t)compLen, gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup
    free(comp);
    free(dest);
    (void)rc_comp;
    (void)rc_validate;
    (void)crc_comp;
    (void)written;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}