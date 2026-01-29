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
//<ID> 917
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for compression and gz write.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    int def_end = deflateEnd(&strm);

    // step 4: Validate / Cleanup
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, (const char *)source);
    off_t offset = gzoffset(gzf);
    int gz_cl = gzclose(gzf);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    uLong crc_val = crc32(0L, source, (uInt)src_len);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end;
    (void)gz_put_ret;
    (void)offset;
    (void)gz_cl;
    (void)first_crc;
    (void)crc_val;
    // API sequence test completed successfully
    return 66;
}