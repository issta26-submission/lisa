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
//<ID> 1159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize data, deflate stream and compress source
    const Bytef source[] = "zlib API sequence payload for gzsetparams/inflateSetDictionary/get_crc_table/deflatePending/inflateBackEnd usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;

    // step 2: Persist original source into a .gz file and configure gz stream parameters
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzset_ret = gzsetparams(gw, 9, 0);
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);

    // step 3: Initialize inflate-back stream, set dictionary and query deflate pending bytes
    z_stream infb_strm;
    memset(&infb_strm, 0, (int)sizeof(infb_strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int infb_init_ret = inflateBackInit_(&infb_strm, 15, window, version, (int)sizeof(z_stream));
    int inf_setdict_ret = inflateSetDictionary(&infb_strm, source, (uInt)source_len);
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending_bytes, &pending_bits);

    // step 4: Validate using CRC table, clean up streams and free resources
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t table_first = crc_table ? crc_table[0] : 0;
    uLong checksum = crc32(0UL, comp_buf, (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size));
    checksum ^= (uLong)table_first;
    int infb_end_ret = inflateBackEnd(&infb_strm);
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_ret = gzclose(gw);
    free(comp_buf);
    free(window);

    (void)def_init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gzset_ret;
    (void)gzwrite_ret;
    (void)infb_init_ret;
    (void)inf_setdict_ret;
    (void)pending_ret;
    (void)pending_bytes;
    (void)pending_bits;
    (void)crc_table;
    (void)table_first;
    (void)checksum;
    (void)infb_end_ret;
    (void)def_end_ret;
    (void)gzclose_ret;
    (void)version;
    (void)bound;
    (void)source_len;
    return 66; // API sequence test completed successfully
}