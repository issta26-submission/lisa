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
//<ID> 1311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib fuzz payload for API sequence test";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong def_bound = deflateBound(&def_strm, src_len);
    Bytef *def_comp = (Bytef *)malloc((size_t)def_bound);
    memset(def_comp, 0, (int)def_bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = def_comp;
    def_strm.avail_out = (uInt)(def_bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)def_bound);
    int defret = deflate(&def_strm, 4);
    uLong def_comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 2: Configure and compress
    uLong bound2 = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound2);
    memset(comp_buf, 0, (int)bound2);
    uLongf comp_len = (uLongf)bound2;
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);

    // step 3: Operate with gz (write original data, then read it back)
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int close_w = gzclose(gz_w);
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)src_len);
    memset(read_buf, 0, (int)src_len);
    int gr = gzread(gz_r, (voidp)read_buf, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int close_r = gzclose(gz_r);

    // step 4: Validate and cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = def_comp;
    inf_strm.avail_in = (uInt)(def_comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)def_comp_size);
    uLong out_len = src_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_len);
    memset(out_buf, 0, (int)out_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_len);
    int inf_ret = inflate(&inf_strm, 4);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int inf_end = inflateEnd(&inf_strm);

    free(def_comp);
    free(comp_buf);
    free(read_buf);
    free(out_buf);

    (void)def_init;
    (void)def_bound;
    (void)defret;
    (void)def_comp_size;
    (void)def_end_ret;
    (void)bound2;
    (void)comp_len;
    (void)comp_ret;
    (void)gw;
    (void)close_w;
    (void)gr;
    (void)close_r;
    (void)inf_init;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}