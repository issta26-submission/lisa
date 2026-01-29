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
//<ID> 1566
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflateBound -> deflateCopy -> deflate -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong cbound = compressBound(src_len);
    Bytef *alt_buf = (Bytef *)malloc((size_t)cbound);
    memset(alt_buf, 0, (int)cbound);
    z_stream defstrm;
    z_stream defcopy;
    z_stream infstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    memset(&defcopy, 0, (int)sizeof(defcopy));
    memset(&infstrm, 0, (int)sizeof(infstrm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound_est = deflateBound(&defstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound_est);
    memset(comp_buf, 0, (int)bound_est);
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)src_len;
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)bound_est;
    int copy_ret = deflateCopy(&defcopy, &defstrm);

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4); // Z_FINISH == 4
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)defstrm.total_out;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    infstrm.next_out = out_buf;
    infstrm.avail_out = (uInt)src_len;
    const char *ver2 = zlibVersion();
    int inf_init_ret = inflateInit_(&infstrm, ver2, (int)sizeof(z_stream));
    int inf_ret = inflate(&infstrm, 0);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&infstrm);
    int def_end_ret = deflateEnd(&defstrm);
    int defcopy_end_ret = deflateEnd(&defcopy);
    free(alt_buf);
    free(comp_buf);
    free(out_buf);
    (void)cbound;
    (void)def_init_ret;
    (void)bound_est;
    (void)copy_ret;
    (void)def_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)defcopy_end_ret;
    (void)ver;
    (void)ver2;
    (void)src_len;
    // API sequence test completed successfully
    return 66;
}