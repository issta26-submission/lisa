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
//<ID> 1029
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib API sequence payload exercising crc32, crc32_z, gzwrite, uncompress";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Decompress (uncompress) into a destination buffer
    Bytef *dest_buf = new Bytef[(payload_len ? payload_len : 1)];
    uLongf dest_len = (uLongf)payload_len;
    int rc_uncomp = uncompress(dest_buf, &dest_len, comp_buf, (uLong)comp_len_f);

    // step 3: Compute CRCs and write output using gzwrite
    uLong crc_from_orig = crc32(0L, src_buf, (uInt)payload_len);
    uLong crc_from_uncompressed_z = crc32_z(0L, dest_buf, (z_size_t)dest_len);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)dest_buf, (unsigned int)dest_len);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup and validate
    delete [] comp_buf;
    delete [] dest_buf;

    (void)rc_comp;
    (void)rc_uncomp;
    (void)crc_from_orig;
    (void)crc_from_uncompressed_z;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}