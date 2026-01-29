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
//<ID> 898
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for deflateCopy, gzputs, and CRC table usage.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream strm_src;
    z_stream strm_copy;
    memset(&strm_src, 0, sizeof(strm_src));
    memset(&strm_copy, 0, sizeof(strm_copy));
    int init_ret = deflateInit_(&strm_src, 6, version, (int)sizeof(z_stream));
    strm_src.next_in = (Bytef *)source;
    strm_src.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong out_bound = deflateBound(&strm_src, src_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    strm_src.next_out = out_buf;
    strm_src.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&strm_src, 4);
    int copy_ret = deflateCopy(&strm_copy, &strm_src);
    // Give the copied stream its own output buffer
    uLong copy_out_bound = deflateBound(&strm_copy, 0);
    Bytef *copy_out_buf = (Bytef *)malloc((size_t)(copy_out_bound ? copy_out_bound : 1));
    memset(copy_out_buf, 0, (size_t)(copy_out_bound ? copy_out_bound : 1));
    strm_copy.next_out = copy_out_buf;
    strm_copy.avail_out = (uInt)(copy_out_bound ? copy_out_bound : 1);
    int def_copy_ret = deflate(&strm_copy, 4);

    gzFile gzf = gzopen64("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, "gzputs: writing a line to gz file\n");
    off64_t gz_off = gzoffset64(gzf);
    int gz_cl_ret = gzclose(gzf);

    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl ? crc_tbl[0] : (z_crc_t)0;

    // step 4: Validate / Cleanup
    int end_ret_src = deflateEnd(&strm_src);
    int end_ret_copy = deflateEnd(&strm_copy);
    free(out_buf);
    free(copy_out_buf);
    (void)version;
    (void)init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)copy_ret;
    (void)def_copy_ret;
    (void)gz_put_ret;
    (void)gz_off;
    (void)gz_cl_ret;
    (void)first_crc;
    (void)end_ret_src;
    (void)end_ret_copy;
    // API sequence test completed successfully
    return 66;
}