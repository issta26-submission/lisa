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
//<ID> 1150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, dictionary, initialize deflate and produce compressed buffer
    const Bytef source[] = "zlib API sequence fuzz driver payload demonstrating required API usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const Bytef dict[] = "preset_dictionary_for_inflate";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    unsigned int pending_bits_count = 0;
    int pending_bits_val = 0;
    int pending_ret = deflatePending(&def_strm, &pending_bits_count, &pending_bits_val);
    int def_end_ret = deflateEnd(&def_strm);

    // step 2: Persist original source into a .gz file and adjust parameters
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzset_ret = gzsetparams(gw, 6, 0);
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_ret = gzclose(gw);

    // step 3: Initialize inflate stream, set dictionary, perform inflate and consult CRC table
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int dict_set_ret = inflateSetDictionary(&inf_strm, dict, dict_len);
    uLong out_alloc = source_len + 256;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > (uLong)0xFFFFFFFFU ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc_entry = crc_tbl ? crc_tbl[0] : (z_crc_t)0;
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Demonstrate inflateBackInit_/inflateBackEnd usage and final cleanup
    z_stream back_strm;
    memset(&back_strm, 0, (int)sizeof(back_strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int back_init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int back_end_ret = inflateBackEnd(&back_strm);
    free(window);
    free(comp_buf);
    free(out_buf);

    (void)def_init_ret;
    (void)def_ret;
    (void)pending_ret;
    (void)pending_bits_count;
    (void)pending_bits_val;
    (void)def_end_ret;
    (void)gzset_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)inf_init_ret;
    (void)dict_set_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)out_size;
    (void)first_crc_entry;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)version;
    (void)bound;
    (void)source_len;

    // API sequence test completed successfully
    return 66;
}