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
//<ID> 1280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: write via gzwrite, seek with gzseek, compress with compress2 and inflate to query inflateCodesUsed";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src_data, src_len, 6);
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzf_w, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    off_t gz_pos_after_write = gzseek(gzf_w, 0, 0);

    // step 2: Configure inflate stream
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int inf_init_ret = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);

    // step 3: Operate (inflate and query), then use gzseek on read handle
    int inf_ret = inflate(&strm, 4);
    unsigned long codes_used = inflateCodesUsed(&strm);
    int inf_end_ret = inflateEnd(&strm);
    int gzclose_w_ret = gzclose_w(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t gz_pos_read_start = gzseek(gzf_r, 0, 0);
    int gzclose_r_ret = gzclose_r(gzf_r);

    // step 4: Validate and cleanup
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)bound;
    (void)comp_ret;
    (void)comp_len;
    (void)gz_write_ret;
    (void)gz_pos_after_write;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)gzclose_w_ret;
    (void)gz_pos_read_start;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}