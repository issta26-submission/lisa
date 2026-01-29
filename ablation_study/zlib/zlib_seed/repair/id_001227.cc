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
//<ID> 1227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload demonstrating deflate, gzip write, inflateBack init/end and crc table access.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    z_stream back_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&back_strm, 0, (int)sizeof(back_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure deflate stream and perform compression
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int deflate_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Persist compressed data with gz API and initialize inflateBack for cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzwrite_ret = gz ? gzwrite(gz, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size)) : 0;
    int gzclose_ret = gz ? gzclose(gz) : 0;
    const z_crc_t *crc_table = get_crc_table();
    uLong sample_table_val = (uLong)(crc_table ? crc_table[0] : 0U);
    unsigned char *window_buf = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window_buf, 0, (int)(1 << 15));
    int back_init_ret = inflateBackInit_(&back_strm, 15, window_buf, version, (int)sizeof(z_stream));
    int back_end_ret = inflateBackEnd(&back_strm);

    // step 4: Cleanup and finalization
    free(comp_buf);
    free(window_buf);
    (void)version;
    (void)def_init_ret;
    (void)deflate_ret;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)sample_table_val;
    (void)back_init_ret;
    (void)back_end_ret;
    // API sequence test completed successfully
    return 66;
}