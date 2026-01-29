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
//<ID> 1221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload for deflate -> write -> inflateBack demo.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream back_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&back_strm, 0, (int)sizeof(back_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure deflate stream and compress data
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

    // step 3: Initialize inflate-back structures and tear down back-end
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (int)(1 << 15));
    int back_init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int back_end_ret = inflateBackEnd(&back_strm);

    // step 4: Write compressed data to a gz file, validate CRC table, cleanup
    gzFile gf = gzopen("zlib_api_test_out.gz", "wb");
    uInt write_len = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    int gz_wr = gzwrite(gf, comp_buf, write_len);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t crc_first = crc_tbl ? crc_tbl[0] : (z_crc_t)0;
    int gz_cl_ret = gzclose(gf);
    free(comp_buf);
    free(window);
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)gz_wr;
    (void)crc_first;
    (void)gz_cl_ret;
    // API sequence test completed successfully
    return 66;
}