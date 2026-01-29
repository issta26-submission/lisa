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
//<ID> 1304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: write with gzwrite, read with gzgetc, init inflate and combine CRCs";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    uInt first_half = (uInt)(src_len / 2);
    uInt second_half = (uInt)(src_len - first_half);
    uLong crc_part1 = crc32(0L, src_data, first_half);
    uLong crc_part2 = crc32(0L, src_data + first_half, second_half);
    uLong combined_crc = crc32_combine(crc_part1, crc_part2, (off_t)second_half);
    const char *version = zlibVersion();
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure & Operate - write compressed data via gz, then read one byte back
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gzclose_w_ret = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_byte = gzgetc(gz_r);
    int gzclose_r_ret = gzclose(gz_r);

    // step 3: Operate on inflate stream and cleanup inflate backend
    int inf_back_end_ret = inflateBackEnd(&inf_strm);

    // step 4: Cleanup and validate
    (void)crc_part1;
    (void)crc_part2;
    (void)combined_crc;
    (void)version;
    (void)inf_init_ret;
    (void)gz_write_ret;
    (void)gzclose_w_ret;
    (void)first_byte;
    (void)gzclose_r_ret;
    (void)inf_back_end_ret;
    // API sequence test completed successfully
    return 66;
}