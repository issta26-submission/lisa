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
//<ID> 1223
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload demonstrating deflate, inflateBackInit_/inflateBackEnd, CRC table access and gzFile usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure and perform deflate (compression)
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Initialize inflate-backend, inspect CRC table and write compressed data to a gzFile, then end backend
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int back_init_ret = inflateBackInit_(&inf_strm, 15, window, version, (int)sizeof(z_stream));
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gf, comp_buf, (unsigned int)comp_size);
    int gz_close_ret = gzclose(gf);
    int back_end_ret = inflateBackEnd(&inf_strm);

    // step 4: Cleanup and finalization
    free(comp_buf);
    free(window);
    (void)version;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)back_init_ret;
    (void)first_crc;
    (void)gz_write_ret;
    (void)gz_close_ret;
    (void)back_end_ret;
    // API sequence test completed successfully
    return 66;
}