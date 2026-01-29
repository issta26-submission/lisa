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
//<ID> 1026
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    const char payload[] = "zlib API sequence payload exercising crc32, crc32_z, gzwrite, uncompress";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    memset(comp_buf, 0, (size_t)(bound ? bound : 1));
    uLongf comp_len_f = (uLongf)bound;
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Configure (compute CRCs on source)
    uLong crc_start = 0UL;
    uLong crc_v1 = crc32(crc_start, src_buf, (uInt)payload_len);
    uLong crc_v2 = crc32_z(crc_start, src_buf, (z_size_t)payload_len);

    // step 3: Operate (decompress and write to gz file)
    Bytef *uncomp = new Bytef[(payload_len ? payload_len : 1)];
    memset(uncomp, 0, (size_t)(payload_len ? payload_len : 1));
    uLongf uncomp_len_f = (uLongf)payload_len;
    int rc_uncomp = uncompress(uncomp, &uncomp_len_f, comp_buf, (uLong)comp_len_f);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)uncomp, (unsigned int)uncomp_len_f);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and Cleanup
    uLong crc_v3 = crc32(0UL, uncomp, (uInt)uncomp_len_f);
    uLong crc_v4 = crc32_z(0UL, uncomp, (z_size_t)uncomp_len_f);
    delete [] comp_buf;
    delete [] uncomp;
    (void)rc_comp;
    (void)crc_v1;
    (void)crc_v2;
    (void)rc_uncomp;
    (void)gf;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)crc_v3;
    (void)crc_v4;
    // API sequence test completed successfully
    return 66;
}