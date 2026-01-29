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
//<ID> 1020
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    const char payload[] = "zlib API sequence payload exercising crc32_z, gzwrite, crc32, uncompress";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLongf comp_len_f = (uLongf)compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(comp_len_f ? comp_len_f : 1)];
    int rc_comp = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Compute CRCs of original and compressed data
    uLong crc_orig_std = crc32(0UL, src_buf, (uInt)payload_len);
    uLong crc_orig_z = crc32_z(0UL, src_buf, (z_size_t)payload_len);
    uLong crc_comp = crc32(0UL, comp_buf, (uInt)comp_len_f);

    // step 3: Write original payload to a gz file using gzwrite
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)src_buf, (unsigned int)payload_len);
    int rc_gzclose = gzclose(gf);

    // step 4: Decompress with uncompress, compute CRCs on decompressed data, and cleanup
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    uLongf destLen = (uLongf)payload_len;
    int rc_uncomp = uncompress(uncmp_buf, &destLen, comp_buf, (uLong)comp_len_f);
    uLong crc_uncmp_std = crc32(0UL, uncmp_buf, (uInt)destLen);
    uLong crc_uncmp_z = crc32_z(0UL, uncmp_buf, (z_size_t)destLen);

    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)rc_comp;
    (void)crc_orig_std;
    (void)crc_orig_z;
    (void)crc_comp;
    (void)gf;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_uncomp;
    (void)crc_uncmp_std;
    (void)crc_uncmp_z;
    // API sequence test completed successfully
    return 66;
}