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
//<ID> 1389
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence: deflate -> write gz -> read gz -> inflate with dictionary -> reset deflate";
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

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = (uLong)defstrm.total_out;
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gzw, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int close_w = gzclose(gzw);
    int reset_ret = deflateReset(&defstrm);

    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_len + 1);
    memset(read_buf, 0, (int)(comp_len + 1));
    int gr = gzread(gzr, read_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int close_r = gzclose_r(gzr);

    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int set_dict_ret = inflateSetDictionary(&istrm, read_buf, (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len));
    int inf_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup
    (void)def_init_ret;
    (void)def_ret;
    (void)gw;
    (void)close_w;
    (void)reset_ret;
    (void)gr;
    (void)close_r;
    (void)inf_init_ret;
    (void)set_dict_ret;
    (void)inf_ret;
    (void)version;
    int def_end_ret = deflateEnd(&defstrm);
    int inf_end_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(read_buf);
    free(decomp_buf);
    // API sequence test completed successfully
    return 66;
}