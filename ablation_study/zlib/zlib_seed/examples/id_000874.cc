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
//<ID> 874
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib payload for compress and adler";
    uLong sourceLen = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLongf alloc_bound = compressBound(sourceLen);
    Bytef *comp_buf = new Bytef[alloc_bound ? alloc_bound : 1];
    uLongf comp_len = alloc_bound;
    uLong adler_src = adler32_z(0UL, src_buf, (z_size_t)sourceLen);

    // step 2: Setup - open a gzFile on stdout
    gzFile gf = gzdopen(1, "wb");

    // step 3: Core operations - compress, checksum, combine, and write
    int rc_comp = compress2(comp_buf, &comp_len, src_buf, sourceLen, 6);
    uLong adler_comp = adler32_z(0UL, comp_buf, (z_size_t)comp_len);
    uLong combined_adler = adler32_combine(adler_src, adler_comp, (off_t)comp_len);
    int rc_gz_write = gzwrite(gf, (voidpc)src_buf, (unsigned int)sourceLen);

    // step 4: Validate and Cleanup
    (void)combined_adler;
    (void)rc_comp;
    (void)rc_gz_write;
    int rc_gz_close = gzclose(gf);
    delete [] comp_buf;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}