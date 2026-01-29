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
//<ID> 894
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload - using deflateCopy, gzputs, gzoffset64, get_crc_table";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream src_strm;
    memset(&src_strm, 0, (int)sizeof(src_strm));
    int init_ret = deflateInit_(&src_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    src_strm.next_in = (Bytef *)source;
    src_strm.avail_in = (uInt)src_len;
    uLong out_bound = deflateBound(&src_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (int)out_bound);
    src_strm.next_out = comp_buf;
    src_strm.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&src_strm, 4);
    uLong comp_len = src_strm.total_out;
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = deflateCopy(&copy_strm, &src_strm);
    int end_copy_ret = deflateEnd(&copy_strm);
    int end_src_ret = deflateEnd(&src_strm);

    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gzputs_ret = gzputs(gzf, (const char *)source);
    off64_t offset = gzoffset64(gzf);
    int gzclose_ret = gzclose(gzf);

    const z_crc_t *crc_table = get_crc_table();
    z_crc_t crc_first = crc_table[0];

    // step 4: Validate / Cleanup
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)end_copy_ret;
    (void)end_src_ret;
    (void)gzputs_ret;
    (void)offset;
    (void)gzclose_ret;
    (void)crc_table;
    (void)crc_first;
    // API sequence test completed successfully
    return 66;
}