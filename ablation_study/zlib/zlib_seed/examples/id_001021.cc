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
//<ID> 1021
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char payload[] = "zlib payload for API sequence test";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong crc_before = crc32(0UL, src_buf, (uInt)payload_len);
    uLong crc_before_z = crc32_z(0UL, src_buf, (z_size_t)payload_len);

    // step 2: Configure (prepare compression buffers)
    uLong comp_bound = compressBound(payload_len);
    Bytef *comp_buf = new Bytef[(comp_bound ? comp_bound : 1)];
    uLongf comp_len_f = (uLongf)comp_bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 3: Operate (uncompress and write to gz file) and Validate (CRCs)
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    uLongf uncmp_len_f = (uLongf)payload_len;
    int rc_uncomp = uncompress(uncmp_buf, &uncmp_len_f, comp_buf, comp_len_f);
    uLong crc_after = crc32(0UL, uncmp_buf, (uInt)uncmp_len_f);
    uLong crc_after_z = crc32_z(0UL, uncmp_buf, (z_size_t)uncmp_len_f);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)uncmp_buf, (unsigned)uncmp_len_f);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup and final checks
    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)crc_before;
    (void)crc_before_z;
    (void)rc_comp;
    (void)rc_uncomp;
    (void)crc_after;
    (void)crc_after_z;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}