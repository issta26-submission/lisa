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
//<ID> 897
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for deflateCopy, gzputs, gzoffset64 and CRC table usage.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    z_stream strm_orig;
    memset(&strm_orig, 0, sizeof(strm_orig));
    int init_ret = deflateInit_(&strm_orig, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    strm_orig.next_in = (Bytef *)source;
    strm_orig.avail_in = (uInt)src_len;
    uLong out_bound = deflateBound(&strm_orig, src_len);
    Bytef *comp_buf1 = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf1, 0, (size_t)out_bound);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf2, 0, (size_t)out_bound);
    z_stream strm_copy;
    memset(&strm_copy, 0, sizeof(strm_copy));
    int copy_ret = deflateCopy(&strm_copy, &strm_orig);
    strm_orig.next_out = comp_buf1;
    strm_orig.avail_out = (uInt)out_bound;
    strm_copy.next_out = comp_buf2;
    strm_copy.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret_orig = deflate(&strm_orig, Z_FINISH);
    int def_ret_copy = deflate(&strm_copy, Z_FINISH);
    uLong comp1_len = strm_orig.total_out;
    uLong comp2_len = strm_copy.total_out;
    gzFile gzf = gzopen("tmp_zlib_sequence.gz", "wb");
    int gzputs_ret = gzputs(gzf, "Zlib sequence header line\n");
    off64_t current_offset = gzoffset64(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    int end_ret_orig = deflateEnd(&strm_orig);
    int end_ret_copy = deflateEnd(&strm_copy);
    free(comp_buf1);
    free(comp_buf2);
    (void)version;
    (void)init_ret;
    (void)first_crc;
    (void)out_bound;
    (void)copy_ret;
    (void)def_ret_orig;
    (void)def_ret_copy;
    (void)comp1_len;
    (void)comp2_len;
    (void)gzputs_ret;
    (void)current_offset;
    (void)gzclose_ret;
    (void)end_ret_orig;
    (void)end_ret_copy;
    // API sequence test completed successfully
    return 66;
}