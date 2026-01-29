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
//<ID> 1823
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence: compress, copy deflate state, write gz, validate checksum";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    z_stream strm_src;
    z_stream strm_copy;
    memset(&strm_src, 0, (int)sizeof(strm_src));
    memset(&strm_copy, 0, (int)sizeof(strm_copy));
    const char *version = zlibVersion();
    int init_ret = deflateInit_(&strm_src, 6, version, (int)sizeof(z_stream));
    strm_src.next_in = (Bytef *)src_data;
    strm_src.avail_in = (uInt)src_len;
    uLong comp_bound = deflateBound(&strm_src, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    strm_src.next_out = comp_buf;
    strm_src.avail_out = (uInt)comp_bound;
    int def_ret = deflate(&strm_src, 4);
    int copy_ret = deflateCopy(&strm_copy, &strm_src);

    // step 2: Configure & Operate
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int put1 = gzputs(gzf, (const char *)src_data);
    int put2 = gzputs(gzf, "\n-- end of original text --\n");
    gzclearerr(gzf);

    // step 3: Validate
    uLong checksum = adler32(0UL, src_data, (uInt)src_len);

    // step 4: Cleanup
    int end_src = deflateEnd(&strm_src);
    int end_copy = deflateEnd(&strm_copy);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)copy_ret;
    (void)put1;
    (void)put2;
    (void)checksum;
    (void)end_src;
    (void)end_copy;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}