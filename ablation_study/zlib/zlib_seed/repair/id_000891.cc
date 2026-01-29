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
//<ID> 891
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const char plain_data[] = "zlib fuzz driver test payload for deflateCopy and gz functions";
    uLong data_len = (uLong)(sizeof(plain_data) - 1);
    z_stream src;
    memset(&src, 0, sizeof(src));
    int init_ret = deflateInit_(&src, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    src.next_in = (Bytef *)plain_data;
    src.avail_in = (uInt)data_len;
    uLong out_bound = deflateBound(&src, data_len);
    Bytef *outbuf = (Bytef *)malloc((size_t)out_bound);
    memset(outbuf, 0, (size_t)out_bound);
    src.next_out = outbuf;
    src.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&src, 4);
    z_stream dst;
    memset(&dst, 0, sizeof(dst));
    int copy_ret = deflateCopy(&dst, &src);
    gzFile gzf = gzopen64("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, "gzputs test line\n");
    off64_t offset = gzoffset64(gzf);

    // step 4: Validate / Cleanup
    int end_dst = deflateEnd(&dst);
    int end_src = deflateEnd(&src);
    int gz_cl = gzclose(gzf);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    free(outbuf);
    (void)version;
    (void)init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)copy_ret;
    (void)gz_put_ret;
    (void)offset;
    (void)end_dst;
    (void)end_src;
    (void)gz_cl;
    (void)first_crc;
    // API sequence test completed successfully
    return 66;
}