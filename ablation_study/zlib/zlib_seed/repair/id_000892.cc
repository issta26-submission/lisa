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
//<ID> 892
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for compression and checksum.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)src_len;
    uLong out_bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    z_stream copy;
    memset(&copy, 0, sizeof(copy));
    int copy_ret = deflateCopy(&copy, &strm);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    gzFile gzf = gzopen64("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, (const char *)source);
    int gz_wr_comp = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off64_t offset = gzoffset64(gzf);
    int gz_cl = gzclose(gzf);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&strm);
    int def_end_copy_ret = deflateEnd(&copy);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)first_crc;
    (void)gz_put_ret;
    (void)gz_wr_comp;
    (void)offset;
    (void)gz_cl;
    (void)def_end_ret;
    (void)def_end_copy_ret;
    // API sequence test completed successfully
    return 66;
}