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
//<ID> 1383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence: create deflate stream, compress, write/read with gz, set inflate dictionary and inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);

    // step 2: Configure
    uLong bound = deflateBound(&defstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = (uLong)defstrm.total_out;

    // step 3: Operate
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gzw, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gzflush_ret = gzflush(gzw, 2);
    int gzclose_w_ret = gzclose(gzw);
    int def_reset_ret = deflateReset(&defstrm);

    // step 4: Validate & Cleanup
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len ? comp_len : 1));
    memset(read_buf, 0, (int)(comp_len ? comp_len : 1));
    int gr = gzread(gzr, read_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gzclose_r_ret = gzclose_r(gzr);

    z_stream instrm;
    memset(&instrm, 0, (int)sizeof(instrm));
    int inf_init_ret = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    uInt dict_len = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    int set_ret = inflateSetDictionary(&instrm, src, dict_len);
    instrm.next_in = comp_buf;
    instrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    instrm.next_out = decomp_buf;
    instrm.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&instrm, 0);
    uLong decomp_written = (uLong)instrm.total_out;

    int def_end_ret = deflateEnd(&defstrm);
    int inf_end_ret = inflateEnd(&instrm);

    (void)def_init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw;
    (void)gzflush_ret;
    (void)gzclose_w_ret;
    (void)def_reset_ret;
    (void)gr;
    (void)gzclose_r_ret;
    (void)inf_init_ret;
    (void)set_ret;
    (void)inf_ret;
    (void)decomp_written;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)version;

    free(comp_buf);
    free(read_buf);
    free(decomp_buf);
    // API sequence test completed successfully
    return 66;
}