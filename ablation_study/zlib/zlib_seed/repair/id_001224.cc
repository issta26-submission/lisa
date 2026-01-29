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
//<ID> 1224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations, initialize and perform deflate (Initialize → Configure → Operate)
    const Bytef source[] = "zlib API sequence payload demonstrating deflate, inflateBack cleanup, crc table access and gz close.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 2: Initialize inflate-back, query CRC table, write compressed data to gzFile and cleanup (Validate → Cleanup)
    z_stream back_strm;
    memset(&back_strm, 0, (int)sizeof(back_strm));
    unsigned char *window_buf = (unsigned char *)malloc(32768);
    memset(window_buf, 0, 32768);
    int back_init_ret = inflateBackInit_(&back_strm, 15, window_buf, version, (int)sizeof(z_stream));
    int back_end_ret = inflateBackEnd(&back_strm);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_wr = gzwrite(gz, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    int gz_cl_ret = gzclose(gz);

    free(comp_buf);
    free(window_buf);
    (void)version;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)first_crc;
    (void)gz_wr;
    (void)gz_cl_ret;
    (void)crc_tbl;
    // API sequence test completed successfully
    return 66;
}