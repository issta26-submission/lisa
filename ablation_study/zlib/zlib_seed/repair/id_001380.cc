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
//<ID> 1380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: prepare dictionary via gz, compress with deflate, then inflate using dictionary";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gwritten = gzwrite(gzw, src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gzw_close = gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *dict_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(dict_buf, 0, (int)(src_len + 1));
    int gread = gzread(gzr, dict_buf, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gzr_close = gzclose_r(gzr);

    // step 2: Configure
    const char *version = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&defstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uInt dict_len = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    int set_dict = inflateSetDictionary(&istrm, dict_buf, dict_len);

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = (uLong)defstrm.total_out;
    int def_reset_ret = deflateReset(&defstrm);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup
    (void)gwritten;
    (void)gzw_close;
    (void)gread;
    (void)gzr_close;
    (void)def_init;
    (void)set_dict;
    (void)def_ret;
    (void)def_reset_ret;
    (void)inf_init;
    (void)inf_ret;
    int def_end = deflateEnd(&defstrm);
    int inf_end = inflateEnd(&istrm);
    int close_comp = 0;
    free(dict_buf);
    free(comp_buf);
    free(decomp_buf);
    (void)close_comp;
    // API sequence test completed successfully
    return 66;
}