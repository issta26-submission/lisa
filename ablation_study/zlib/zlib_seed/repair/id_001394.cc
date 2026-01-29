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
//<ID> 1394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence: compute crc32 -> compress -> deflate with prime -> write via gz";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong crc_src = crc32(0L, src, (uInt)src_len);
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_len_f = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len_f, src, src_len);
    uLong comp_len = (uLong)comp_len_f;

    // step 2: Configure
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    uLong def_bound = deflateBound(&defstrm, src_len);
    Bytef *def_out = (Bytef *)malloc((size_t)def_bound);
    memset(def_out, 0, (int)def_bound);
    defstrm.next_out = def_out;
    defstrm.avail_out = (uInt)(def_bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)def_bound);
    int prime_ret = deflatePrime(&defstrm, 3, 5);

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong def_len = (uLong)defstrm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int buf_ret = gzbuffer(gz, 8192);
    int gw = gzwrite(gz, def_out, (unsigned int)(def_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)def_len));
    int gzclose_ret = gzclose(gz);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&defstrm);
    uLong crc_comp = crc32(0L, comp_buf, (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len));
    (void)version;
    (void)crc_src;
    (void)comp_ret;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)gw;
    (void)buf_ret;
    (void)gzclose_ret;
    (void)def_end_ret;
    (void)crc_comp;
    free(comp_buf);
    free(def_out);
    // API sequence test completed successfully
    return 66;
}