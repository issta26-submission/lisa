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
//<ID> 1309
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef part1[] = "Hello ";
    const Bytef part2[] = "zlib!";
    uLong len1 = (uLong)(sizeof(part1) - 1);
    uLong len2 = (uLong)(sizeof(part2) - 1);
    uLong crc1 = crc32(0L, part1, (uInt)len1);
    uLong crc2 = crc32(0L, part2, (uInt)len2);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)len2);

    // step 2: Configure
    uLong total_src = len1 + len2;
    Bytef *src_buf = (Bytef *)malloc((size_t)total_src);
    memcpy(src_buf, part1, (size_t)len1);
    memcpy(src_buf + len1, part2, (size_t)len2);
    uLong bound = compressBound(total_src);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src_buf, total_src, 6);

    // step 3: Operate (write compressed data to a gz file, then read a byte)
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int close_w_ret = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_byte = gzgetc(gz_r);
    int close_r_ret = gzclose(gz_r);

    // step 4: Validate & Cleanup
    const char *version = zlibVersion();
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    uLong out_len = total_src + 16;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_len);
    memset(out_buf, 0, (int)out_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_len);
    int inf_ret = inflate(&inf_strm, 4);
    int inf_back_ret = inflateBackEnd(&inf_strm);

    free(src_buf);
    free(comp_buf);
    free(out_buf);

    (void)crc1;
    (void)crc2;
    (void)combined_crc;
    (void)comp_ret;
    (void)bound;
    (void)gw;
    (void)close_w_ret;
    (void)first_byte;
    (void)close_r_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_back_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}