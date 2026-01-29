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
//<ID> 1023
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    const char payload[] = "zlib API sequence payload for compress/uncompress and CRC/gzwrite testing";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong crc_initial32 = crc32(0UL, src_buf, (uInt)payload_len);
    uLong crc_initial32_z = crc32_z(0UL, src_buf, (z_size_t)payload_len);

    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? bound : 1)];
    uLongf comp_len_f = (uLongf)bound;
    int rc_compress = compress2(comp_buf, &comp_len_f, src_buf, (uLong)payload_len, 6);

    // step 2: Configure uncompress destination
    Bytef *uncmp_buf = new Bytef[(payload_len ? payload_len : 1)];
    uLongf uncmp_len_f = (uLongf)payload_len;
    int rc_uncompress = uncompress(uncmp_buf, &uncmp_len_f, comp_buf, (uLong)comp_len_f);

    // step 3: Operate — compute CRCs on uncompressed data and write with gzwrite
    uLong crc_after32 = crc32(0UL, uncmp_buf, (uInt)uncmp_len_f);
    uLong crc_after32_z = crc32_z(0UL, uncmp_buf, (z_size_t)uncmp_len_f);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)uncmp_buf, (unsigned int)uncmp_len_f);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate & Cleanup
    delete [] comp_buf;
    delete [] uncmp_buf;

    (void)crc_initial32;
    (void)crc_initial32_z;
    (void)rc_compress;
    (void)rc_uncompress;
    (void)crc_after32;
    (void)crc_after32_z;
    (void)rc_gzwrite;
    (void)rc_gzclose;

    // API sequence test completed successfully
    return 66;
}