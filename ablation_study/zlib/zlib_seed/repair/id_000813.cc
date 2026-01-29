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
//<ID> 813
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare data and compress with deflate)
    const char *version = zlibVersion();
    const char msg[] = "Hello zlib!";
    uLong msg_len = (uLong)(sizeof(msg) - 1);

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = (Bytef *)msg;
    def.avail_in = (uInt)msg_len;
    uLong out_bound = deflateBound(&def, msg_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    int def_end_ret = deflateEnd(&def);

    // step 2: Configure (write the compressed bytes into a gzip file with custom params)
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gz_set_ret = gzsetparams(gzf_w, 6, 0);
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_cl_w = gzclose(gzf_w);

    // step 3: Operate (read back the written data via gzread into a buffer)
    gzFile gzf_r = gzopen(fname, "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_len);
    memset(read_buf, 0, (size_t)comp_len);
    int gz_rd = gzread(gzf_r, read_buf, (unsigned int)comp_len);
    int gz_cl_r = gzclose(gzf_r);

    // step 4: Validate / Cleanup (decompress the read data with uncompress, initialize inflateBack, then cleanup)
    uLongf dest_len = (uLongf)msg_len;
    Bytef *dest = (Bytef *)malloc((size_t)dest_len);
    memset(dest, 0, (size_t)dest_len);
    int un_ret = uncompress(dest, &dest_len, read_buf, (uLong)comp_len);

    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int back_init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    int inf_end_ret = inflateEnd(&inf);

    int back_end_ret = inflateBackEnd(&back_strm);

    free(comp_buf);
    free(read_buf);
    free(dest);

    (void)version;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gz_set_ret;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)gz_cl_r;
    (void)un_ret;
    (void)back_init_ret;
    (void)inf_init_ret;
    (void)inf_end_ret;
    (void)back_end_ret;
    (void)fname;
    // API sequence test completed successfully
    return 66;
}