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
//<ID> 1391
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence: compress -> deflatePrime -> deflate -> write gz -> crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);
    uLong crc_before = crc32(0L, src, (uInt)src_len);

    // step 2: Configure
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = comp_buf;
    defstrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(out_buf, 0, (int)(src_len + 64));
    defstrm.next_out = out_buf;
    defstrm.avail_out = (uInt)((src_len + 64) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 64));
    int prime_ret = deflatePrime(&defstrm, 3, 1);

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong def_total_out = defstrm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gb = gzbuffer(gz, 65536u);
    int gw = gzwrite(gz, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gzclose_ret = gzclose(gz);

    // step 4: Validate & Cleanup
    uLong crc_after = crc32(crc_before, out_buf, (uInt)(def_total_out > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)def_total_out));
    int def_end_ret = deflateEnd(&defstrm);
    (void)comp_ret;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)def_total_out;
    (void)gb;
    (void)gw;
    (void)gzclose_ret;
    (void)crc_after;
    (void)crc_before;
    (void)version;
    free(comp_buf);
    free(out_buf);
    // API sequence test completed successfully
    return 66;
}