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
//<ID> 1565
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress -> deflateInit_/deflateBound/deflateCopy -> inflate -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);

    // step 2: Configure
    z_stream defstrm;
    z_stream defcopy;
    z_stream infstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    memset(&defcopy, 0, (int)sizeof(defcopy));
    memset(&infstrm, 0, (int)sizeof(infstrm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    uLong def_bound = deflateBound(&defstrm, src_len);
    Bytef *def_out_buf = (Bytef *)malloc((size_t)def_bound);
    memset(def_out_buf, 0, (int)def_bound);
    int copy_ret = deflateCopy(&defcopy, &defstrm);
    int inf_init_ret = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    infstrm.next_out = out_buf;
    infstrm.avail_out = (uInt)src_len;

    // step 3: Operate
    int inf_ret = inflate(&infstrm, 0);
    uLong consumed_in = infstrm.total_in;
    uLong produced_out = infstrm.total_out;
    uLong def_bound_again = deflateBound(&defcopy, comp_len);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&infstrm);
    int def_end_ret = deflateEnd(&defstrm);
    int defcopy_end_ret = deflateEnd(&defcopy);
    free(comp_buf);
    free(def_out_buf);
    free(out_buf);
    (void)comp_ret;
    (void)def_init_ret;
    (void)copy_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)consumed_in;
    (void)produced_out;
    (void)def_bound_again;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)defcopy_end_ret;
    // API sequence test completed successfully
    return 66;
}