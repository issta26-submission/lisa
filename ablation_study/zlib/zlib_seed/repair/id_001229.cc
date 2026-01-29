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
//<ID> 1229
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload demonstrating deflate, inflate-back init/end, crc table and gz close usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    const z_crc_t *crc_table = get_crc_table();
    z_stream def_strm;
    z_stream inf_strm;
    z_stream back_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    memset(&back_strm, 0, (int)sizeof(back_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure and perform deflate (compression)
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

    // step 3: Initialize inflate and inflate-back, write compressed data to a gz file
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 512;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (int)out_alloc);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_alloc > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_alloc);
    int inf_ret = inflate(&inf_strm, 4);
    uLong inflated_size = inf_strm.total_out;
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int back_init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzf ? gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size)) : 0;

    // step 4: Validation and cleanup
    int back_end_ret = inflateBackEnd(&back_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    if (gzf) { int gzclose_ret = gzclose(gzf); (void)gzclose_ret; }
    free(comp_buf);
    free(out_buf);
    free(window);
    (void)version;
    (void)crc_table;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inflated_size;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)gz_write_ret;
    (void)bound;
    // API sequence test completed successfully
    return 66;
}