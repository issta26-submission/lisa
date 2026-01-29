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
//<ID> 1385
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib sequence: deflate -> write gz -> read gz -> inflate with dictionary and reset deflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&dstrm, 4);
    uLong comp_len = (uLong)dstrm.total_out;

    // step 2: Configure / reuse deflate
    int def_reset_ret = deflateReset(&dstrm);

    // step 3: Write compressed data to gz and read it back
    gzFile gw = gzopen("test_zlib_seq.gz", "wb");
    int gw_ret = gzwrite(gw, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_seq.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len + 1));
    memset(read_buf, 0, (int)(comp_len + 1));
    int gr_ret = gzread(gr, read_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gr_close = gzclose_r(gr);

    // step 4: Inflate using the read buffer as dictionary, operate, validate & cleanup
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uInt dict_len = (uInt)(gr_ret > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)gr_ret);
    int set_dict_ret = inflateSetDictionary(&istrm, read_buf, dict_len);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&istrm, 0);
    int inf_end_ret = inflateEnd(&istrm);
    int def_end_ret = deflateEnd(&dstrm);
    free(comp_buf);
    free(read_buf);
    free(decomp_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_reset_ret;
    (void)gw_ret;
    (void)gw_close;
    (void)gr_ret;
    (void)gr_close;
    (void)inf_init_ret;
    (void)set_dict_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}