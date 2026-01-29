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
//<ID> 1287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize - prepare source and compress it with zlib's compress2 (zlib format)
    const Bytef src[] = "zlib API sequence payload: produce zlib-compressed bytes, store in gzip container, read back and inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len_f = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len_f, src, src_len, 6);

    // step 2: Configure and operate - write the zlib-compressed bytes into a gzip file, seek and close writer
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzf_w, comp_buf, (unsigned int)(comp_len_f > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len_f));
    off_t pos_after_write = gzseek(gzf_w, 0, 0);
    int gzclose_w_ret = gzclose_w(gzf_w);

    // step 3: Operate - reopen for reading, read back the gzip-decompressed payload (which is the zlib-compressed bytes), then inflate them
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t pos_after_open = gzseek(gzf_r, 0, 0);
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_len_f);
    memset(read_buf, 0, (int)comp_len_f);
    int gz_read_ret = gzread(gzf_r, (voidp)read_buf, (unsigned int)(comp_len_f > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len_f));
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)(gz_read_ret < 0 ? 0U : (uInt)gz_read_ret);
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    int inf_ret = inflate(&inf_strm, 4);
    uLong inflated_size = inf_strm.total_out;
    unsigned long codes_used = inflateCodesUsed(&inf_strm);

    // step 4: Validate & Cleanup - finalize inflate, close reader, free buffers and return
    int inf_end_ret = inflateEnd(&inf_strm);
    int gzclose_r_ret = gzclose_r(gzf_r);
    free(comp_buf);
    free(read_buf);
    free(out_buf);
    (void)comp_ret;
    (void)bound;
    (void)comp_len_f;
    (void)gz_write_ret;
    (void)pos_after_write;
    (void)pos_after_open;
    (void)gz_read_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inflated_size;
    (void)codes_used;
    (void)inf_end_ret;
    (void)gzclose_w_ret;
    (void)gzclose_r_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}