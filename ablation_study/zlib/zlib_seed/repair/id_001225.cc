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
//<ID> 1225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload for deflate/inflateBack and gz I/O demonstration.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream back_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&back_strm, 0, (int)sizeof(back_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    unsigned char window[32768];
    int back_init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));

    // step 2: Configure compression buffers and CRC table usage
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t crc_first = crc_tbl[0];

    // step 3: Operate (compress) and write compressed data to a gz file
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gz = gzopen("zlib_seq_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gz, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size));
    int gzclose_ret = gzclose(gz);

    // step 4: Validation/Cleanup (inflateBackEnd for back_strm, end deflate, free buffers)
    int back_end_ret = inflateBackEnd(&back_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)back_init_ret;
    (void)crc_first;
    (void)def_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)back_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}