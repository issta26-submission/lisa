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
//<ID> 456
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateInit_ -> inflate -> validate -> write gz";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream and perform inflate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen * 2 + 16;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    strm.next_out = dest;
    strm.avail_out = (uInt)outCap;
    int rc_inflate = inflate(&strm, 0);

    // step 3: Validate inflate state and compute checksum
    int rc_validate = inflateValidate(&strm, 1);
    uLong out_size = strm.total_out;
    uLong crc = crc32_z(0UL, dest, (z_size_t)out_size);

    // step 4: Write decompressed data to a gz file and cleanup
    gzFile gf = gzopen64("zlib_api_sequence_out.gz", "wb");
    z_size_t rc_gzfwrite = gzfwrite((voidpc)dest, (z_size_t)1, (z_size_t)out_size, gf);
    int rc_gzclose = gzclose(gf);
    int rc_inf_end = inflateEnd(&strm);
    free(comp);
    free(dest);
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_validate;
    (void)crc;
    (void)rc_gzfwrite;
    (void)rc_gzclose;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}