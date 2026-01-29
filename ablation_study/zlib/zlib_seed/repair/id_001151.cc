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
//<ID> 1151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data, dictionary, deflate stream and allocate compression buffer
    const Bytef source[] = "zlib API sequence fuzz driver payload demonstrating deflatePending/gzsetparams/inflateSetDictionary usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const Bytef dict[] = "preset_dictionary_for_inflate";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);

    // step 2: Compress the source into comp_buf, query pending bits, finalize deflate, then persist original into .gz with new params
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    unsigned int pending = 0;
    int bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending, &bits);
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzset_ret = gzsetparams(gw, 9, 0);
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_ret = gzclose(gw);

    // step 3: Initialize inflate stream, set dictionary, inflate compressed buffer into out_buf and validate with CRC and CRC table
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int inf_dict_ret = inflateSetDictionary(&inf_strm, dict, dict_len);
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (int)out_alloc);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > (uLong)0xFFFFFFFFU ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    uLong checksum = crc32(0UL, out_buf, (uInt)out_size);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t table_val = crc_table ? crc_table[0] : (z_crc_t)0;
    uLong combined_validate = checksum ^ (uLong)table_val;

    // step 4: Cleanup inflate back-end and final resources
    int inf_back_end_ret = inflateBackEnd(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(out_buf);

    (void)def_init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)pending_ret;
    (void)pending;
    (void)bits;
    (void)def_end_ret;
    (void)gw;
    (void)gzset_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)inf_init_ret;
    (void)inf_dict_ret;
    (void)inf_ret;
    (void)checksum;
    (void)table_val;
    (void)combined_validate;
    (void)inf_back_end_ret;
    (void)inf_end_ret;
    (void)version;
    (void)bound;
    (void)source_len;
    (void)out_size;
    return 66;
    // API sequence test completed successfully
}