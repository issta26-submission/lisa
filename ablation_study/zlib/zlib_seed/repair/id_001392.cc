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
//<ID> 1392
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence: compress -> deflatePrime -> write gz -> crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_orig = crc32(0UL, src, (uInt)src_len);
    uLong bound1 = compressBound(src_len);
    Bytef *comp_buf1 = (Bytef *)malloc((size_t)bound1);
    memset(comp_buf1, 0, (int)bound1);
    uLongf comp1_len_f = (uLongf)bound1;
    int comp_ret = compress(comp_buf1, &comp1_len_f, src, src_len);
    uLong comp1_len = (uLong)comp1_len_f;

    // step 2: Configure
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = comp_buf1;
    defstrm.avail_in = (uInt)(comp1_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp1_len);
    uLong bound2 = deflateBound(&defstrm, comp1_len);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)bound2);
    memset(comp_buf2, 0, (int)bound2);
    defstrm.next_out = comp_buf2;
    defstrm.avail_out = (uInt)(bound2 > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound2);

    // step 3: Operate
    int prime_ret = deflatePrime(&defstrm, 3, 5);
    int def_ret = deflate(&defstrm, 4);
    uLong comp2_len = (uLong)defstrm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int buf_ret = gzbuffer(gzf, 16384U);
    int gw = gzwrite(gzf, comp_buf2, (unsigned int)(comp2_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp2_len));
    int close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    uLong crc_comp2 = crc32(0UL, comp_buf2, (uInt)(comp2_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp2_len));
    (void)comp_ret;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)gw;
    (void)buf_ret;
    (void)close_ret;
    (void)crc_orig;
    (void)crc_comp2;
    int def_end_ret = deflateEnd(&defstrm);
    free(comp_buf1);
    free(comp_buf2);
    // API sequence test completed successfully
    return 66;
}