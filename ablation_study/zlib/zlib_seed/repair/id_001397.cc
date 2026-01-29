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
//<ID> 1397
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: compress -> deflate with prime -> write via gz with buffer -> crc32 checks";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_orig = crc32(0UL, src, (uInt)src_len);

    // step 2: Configure
    uLongf comp1_len = 0UL;
    uLong comp1_bound = compressBound(src_len);
    Bytef *comp1_buf = (Bytef *)malloc((size_t)comp1_bound);
    memset(comp1_buf, 0, (int)comp1_bound);
    int comp_ret = compress(comp1_buf, &comp1_len, src, src_len);

    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    uLong comp2_bound = deflateBound(&dstrm, src_len);
    Bytef *comp2_buf = (Bytef *)malloc((size_t)comp2_bound);
    memset(comp2_buf, 0, (int)comp2_bound);
    dstrm.next_out = comp2_buf;
    dstrm.avail_out = (uInt)(comp2_bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp2_bound);

    // step 3: Operate
    int prime_ret = deflatePrime(&dstrm, 3, 5);
    int def_ret = deflate(&dstrm, 4);
    uLong comp2_len = (uLong)dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gb = gzbuffer(gz, 8192);
    int gw = gzwrite(gz, comp2_buf, (unsigned int)(comp2_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp2_len));
    int gf = gzflush(gz, 2);
    int gz_close_ret = gzclose(gz);

    // step 4: Validate & Cleanup
    uLong crc_comp1 = crc32(0UL, comp1_buf, (uInt)(comp1_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp1_len));
    uLong crc_comp2 = crc32(0UL, comp2_buf, (uInt)(comp2_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp2_len));
    (void)version;
    (void)comp_ret;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gb;
    (void)gw;
    (void)gf;
    (void)gz_close_ret;
    free(comp1_buf);
    free(comp2_buf);
    // API sequence test completed successfully
    return 66;
}