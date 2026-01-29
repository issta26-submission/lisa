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
//<ID> 890
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence payload for deflateCopy, gzputs and crc table usage";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream src;
    memset(&src, 0, sizeof(src));
    int init_ret = deflateInit_(&src, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    src.next_in = (Bytef *)source;
    src.avail_in = (uInt)source_len;
    uLong out_bound = deflateBound(&src, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    src.next_out = comp_buf;
    src.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&src, 4);
    uLong comp_len = src.total_out;
    z_stream dest;
    memset(&dest, 0, sizeof(dest));
    int copy_ret = deflateCopy(&dest, &src);
    dest.next_out = comp_buf + comp_len; /* point to remainder of buffer for demonstration */
    dest.avail_out = (uInt)(out_bound - comp_len);
    /* touch CRC table and store first entry into stream adler for meaningful data flow */
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_tbl = crc_tbl[0];
    src.adler = (uLong)first_tbl;

    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, (const char *)source);
    off64_t offset = gzoffset64(gzf);
    int gz_cl_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    int dest_end = deflateEnd(&dest);
    int src_end = deflateEnd(&src);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)first_tbl;
    (void)crc_tbl;
    (void)gz_put_ret;
    (void)offset;
    (void)gz_cl_ret;
    (void)dest_end;
    (void)src_end;
    // API sequence test completed successfully
    return 66;
}