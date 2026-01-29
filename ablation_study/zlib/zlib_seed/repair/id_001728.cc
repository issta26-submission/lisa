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
//<ID> 1728
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence: deflate -> inflate -> crc32 -> gzwrite -> gzseek64/gzoffset";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong orig_crc = crc32(0UL, src, (uInt)src_len);
    z_stream dstr;
    memset(&dstr, 0, (int)sizeof(dstr));
    int def_init_ret = deflateInit_(&dstr, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    dstr.next_in = (Bytef *)src;
    dstr.avail_in = (uInt)src_len;
    dstr.next_out = comp_buf;
    dstr.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&dstr, 4);
    uLong comp_len = dstr.total_out;
    int def_end_ret = deflateEnd(&dstr);

    // step 4: Validate & Cleanup
    z_stream istr;
    memset(&istr, 0, (int)sizeof(istr));
    int inf_init_ret = inflateInit_(&istr, ver, (int)sizeof(z_stream));
    istr.next_in = comp_buf;
    istr.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    istr.next_out = decomp_buf;
    istr.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&istr, 4);
    uLong decomp_len = istr.total_out;
    uLong decomp_crc = crc32(0UL, decomp_buf, (uInt)decomp_len);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w_ret = gzwrite(gzf, decomp_buf, (unsigned int)decomp_len);
    off64_t seek_ret = gzseek64(gzf, (off64_t)0, 0);
    off_t offs = gzoffset(gzf);
    int gzclose_ret = gzclose(gzf);
    int inf_end_ret = inflateEnd(&istr);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)orig_crc;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)decomp_crc;
    (void)gz_w_ret;
    (void)seek_ret;
    (void)offs;
    (void)gzclose_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}