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
//<ID> 950
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence payload for deflate/inflate, pending query and checksum combine.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef dict[] = "sample_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    int dict_set_ret = deflateSetDictionary(&def_strm, dict, dict_len);

    // step 3: Operate
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);

    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    Bytef def_out_buf[256];
    memset(def_out_buf, 0, sizeof(def_out_buf));
    def_strm.next_out = def_out_buf;
    def_strm.avail_out = (uInt)sizeof(def_out_buf);
    int def_ret = deflate(&def_strm, 0);
    unsigned int pending = 0;
    int pending_bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending, &pending_bits);

    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    uLong decomp_cap = source_len + 64;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_cap);
    memset(decomp_buf, 0, (size_t)decomp_cap);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)decomp_cap;
    int inf_ret = inflate(&inf_strm, 0);

    // step 4: Validate / Cleanup
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int ib_ret = inflateBackEnd(&inf_strm);
    uLong ad1 = adler32(0L, source, (uInt)source_len);
    uLong ad2 = adler32(0L, decomp_buf, (uInt)source_len);
    uLong combined = adler32_combine64(ad1, ad2, (off64_t)source_len);

    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);

    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)dict_set_ret;
    (void)bound;
    (void)comp_ret;
    (void)def_ret;
    (void)pending;
    (void)pending_bits;
    (void)pending_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)ib_ret;
    (void)ad1;
    (void)ad2;
    (void)combined;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}