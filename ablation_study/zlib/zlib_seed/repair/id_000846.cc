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
//<ID> 846
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef input_data[] = "Example zlib data for testing dictionary and CRC";
    uLong input_len = (uLong)(sizeof(input_data) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = (Bytef *)input_data;
    def.avail_in = (uInt)input_len;
    uLong bound = deflateBound(&def, input_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)(bound + 16));
    memset(comp_buf, 0, (size_t)(bound + 16));
    def.next_out = comp_buf;
    def.avail_out = (uInt)(bound + 16);

    // step 2: Configure
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));
    const Bytef dict[] = "shared_dictionary_for_inflate";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    int inf_set_dict = inflateSetDictionary(&inf, dict, dict_len);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong compressed_len = def.total_out;
    unsigned int pending = 0;
    int pending_bits = 0;
    int pend_ret = deflatePending(&def, &pending, &pending_bits);
    uLong crc = crc32(0UL, input_data, (uInt)input_len);
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gw = gzwrite(gzf_w, comp_buf, (unsigned int)compressed_len);
    int gwc = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    int first_ch = gzgetc(gzf_r);
    int ungot = gzungetc(first_ch, gzf_r);

    // step 4: Validate / Cleanup
    int close_r = gzclose(gzf_r);
    int def_end = deflateEnd(&def);
    int inf_end = inflateEnd(&inf);
    free(comp_buf);
    (void)version;
    (void)def_init;
    (void)bound;
    (void)inf_init;
    (void)inf_set_dict;
    (void)def_ret;
    (void)compressed_len;
    (void)pending;
    (void)pending_bits;
    (void)pend_ret;
    (void)crc;
    (void)fname;
    (void)gw;
    (void)gwc;
    (void)first_ch;
    (void)ungot;
    (void)close_r;
    (void)def_end;
    (void)inf_end;
    // API sequence test completed successfully
    return 66;
}