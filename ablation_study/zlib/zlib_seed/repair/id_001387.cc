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
//<ID> 1387
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: produce compressed data with deflate, write/read via gz, then inflate with a dictionary";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    uLong bound = deflateBound(&defstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = (uLong)defstrm.total_out;

    // step 2: Configure
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gw, src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *dict_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(dict_buf, 0, (int)(src_len + 1));
    int gr_ret = gzread(gr, dict_buf, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gr_close = gzclose_r(gr);

    // step 3: Operate
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&istrm, 0);
    int setdict_ret = inflateSetDictionary(&istrm, dict_buf, (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len));
    int def_reset_ret = deflateReset(&defstrm);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&defstrm);
    int inf_end_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(dict_buf);
    free(decomp_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_ret;
    (void)gw_close;
    (void)gr_ret;
    (void)gr_close;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)setdict_ret;
    (void)def_reset_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}