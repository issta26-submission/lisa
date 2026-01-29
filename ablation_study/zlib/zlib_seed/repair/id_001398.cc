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
//<ID> 1398
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: compute crc32, compress, write with gzbuffer, deflate with prime";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_initial = crc32(0L, src, (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len));
    uLong bound_comp = compressBound(src_len);
    uLongf comp_len_f = (uLongf)bound_comp;
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_len_f);
    memset(comp_buf, 0, (int)comp_len_f);
    int comp_ret = compress(comp_buf, &comp_len_f, src, src_len);
    uLong comp_len = (uLong)comp_len_f;

    // step 2: Configure and write compressed data via gz with buffered IO
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gb_ret = gzbuffer(gzf, 65536U);
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gf_ret = gzflush(gzf, 2);
    int gclose_ret = gzclose(gzf);

    // step 3: Operate - initialize deflate stream, prime bits, perform deflate
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    uLong def_bound = deflateBound(&defstrm, src_len);
    Bytef *def_buf = (Bytef *)malloc((size_t)def_bound);
    memset(def_buf, 0, (int)def_bound);
    defstrm.next_out = def_buf;
    defstrm.avail_out = (uInt)(def_bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)def_bound);
    int prime_ret = deflatePrime(&defstrm, 3, 5);
    int def_ret = deflate(&defstrm, 4);
    uLong deflated_len = (uLong)defstrm.total_out;
    int def_end_ret = deflateEnd(&defstrm);

    // step 4: Validate & Cleanup
    (void)crc_initial;
    (void)comp_ret;
    (void)gb_ret;
    (void)gw_ret;
    (void)gf_ret;
    (void)gclose_ret;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)deflated_len;
    (void)def_end_ret;
    (void)ver;
    free(comp_buf);
    free(def_buf);
    // API sequence test completed successfully
    return 66;
}