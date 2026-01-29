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
//<ID> 1305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "Hello zlib world!";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src_data, src_len, 6);

    // step 2: Configure - write compressed bytes to a gz file and read one char back
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gzclose_w_ret = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int one_char = gzgetc(gz_r);
    int gzclose_r_ret = gzclose(gz_r);

    // step 3: Operate - initialize inflater and inflate from the compressed buffer
    const char *version = zlibVersion();
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    uLong out_len = src_len + 32;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_len);
    memset(out_buf, 0, (int)out_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_len);
    int inf_ret = inflate(&inf_strm, 4);

    // step 4: Validate and cleanup
    uInt half1 = (uInt)(src_len / 2);
    uInt half2 = (uInt)(src_len - half1);
    uLong crc1 = crc32(0UL, src_data, half1);
    uLong crc2 = crc32(0UL, src_data + half1, half2);
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)half2);
    int inf_end_ret = inflateEnd(&inf_strm);
    int inf_back_end_ret = inflateBackEnd(&inf_strm);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)bound;
    (void)gz_write_ret;
    (void)gzclose_w_ret;
    (void)one_char;
    (void)gzclose_r_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)inf_end_ret;
    (void)inf_back_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}