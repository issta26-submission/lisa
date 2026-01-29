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
//<ID> 910
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const char source[] = "Zlib API sequence test payload for compression and file operations.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int init_def = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end = deflateEnd(&def_strm);
    gzFile gz = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gz, source);
    off_t offset = gzoffset(gz);
    int gz_cl_ret = gzclose(gz);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];

    // step 4: Validate / Cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int init_inf = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (size_t)(src_len + 16));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&inf_strm, 4);
    int inf_end = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)init_def;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end;
    (void)gz_put_ret;
    (void)offset;
    (void)gz_cl_ret;
    (void)first_crc;
    (void)init_inf;
    (void)inf_ret;
    (void)inf_end;
    // API sequence test completed successfully
    return 66;
}