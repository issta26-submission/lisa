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
//<ID> 1772
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: initialize -> compress -> inflateInit2_ -> inflate -> adler32 -> gzfwrite -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(comp_buf, 0, (int)bound);
    memset(decomp_buf, 0, (int)(src_len + 64));

    // step 2: Configure (compress source and prepare inflate stream)
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 64);
    int inf_init_ret = inflateInit2_(&inf_strm, 15, ver, (int)sizeof(z_stream));

    // step 3: Operate (inflate, compute checksum, write decompressed data)
    int inf_ret = inflate(&inf_strm, 0);
    uLong checksum = adler32(0UL, decomp_buf, (uInt)src_len);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    z_size_t written = gzfwrite(decomp_buf, 1, (z_size_t)src_len, gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)bound;
    (void)comp_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)checksum;
    (void)written;
    (void)gzclose_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}