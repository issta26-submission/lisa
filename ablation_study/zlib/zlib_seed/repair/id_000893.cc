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
//<ID> 893
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for deflateCopy and gz operations.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    z_stream copy_strm;
    memset(&strm, 0, sizeof(strm));
    memset(&copy_strm, 0, sizeof(copy_strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    int copy_ret = deflateCopy(&copy_strm, &strm);

    // step 2: Configure
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)src_len;
    uLong out_bound = deflateBound(&strm, src_len);
    Bytef *comp1 = (Bytef *)malloc((size_t)out_bound);
    memset(comp1, 0, (size_t)out_bound);
    strm.next_out = comp1;
    strm.avail_out = (uInt)out_bound;
    copy_strm.next_in = (Bytef *)source;
    copy_strm.avail_in = (uInt)src_len;
    Bytef *comp2 = (Bytef *)malloc((size_t)out_bound);
    memset(comp2, 0, (size_t)out_bound);
    copy_strm.next_out = comp2;
    copy_strm.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret1 = deflate(&strm, 4);
    uLong comp1_len = strm.total_out;
    int def_ret2 = deflate(&copy_strm, 4);
    uLong comp2_len = copy_strm.total_out;
    gzFile gzf = gzopen64("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, "Zlib gzputs test line\n");
    off64_t gz_off = gzoffset64(gzf);
    int gz_cl_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    int end_ret1 = deflateEnd(&strm);
    int end_ret2 = deflateEnd(&copy_strm);
    free(comp1);
    free(comp2);
    (void)version;
    (void)init_ret;
    (void)copy_ret;
    (void)out_bound;
    (void)def_ret1;
    (void)comp1_len;
    (void)def_ret2;
    (void)comp2_len;
    (void)gz_put_ret;
    (void)gz_off;
    (void)gz_cl_ret;
    (void)first_crc;
    (void)end_ret1;
    (void)end_ret2;
    // API sequence test completed successfully
    return 66;
}