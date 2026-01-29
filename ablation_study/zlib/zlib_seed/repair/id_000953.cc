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
//<ID> 953
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib api sequence: compress then inflate, query pending and codes used, combine checksums.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure / Compress
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending_bytes, &pending_bits);
    int def_end_ret = deflateEnd(&def_strm);
    uLong comp_len = def_strm.total_out;

    // step 3: Operate / Inflate
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    uLong decomp_cap = source_len + 64;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_cap);
    memset(decomp_buf, 0, (size_t)decomp_cap);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)decomp_cap;
    int inf_ret = inflate(&inf_strm, 0);
    int inf_back_end_ret = inflateBackEnd(&inf_strm);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);

    // step 4: Validate / Cleanup
    uLong combined_adler = adler32_combine64(def_strm.adler, inf_strm.adler, (off64_t)0);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)pending_bytes;
    (void)pending_bits;
    (void)pending_ret;
    (void)def_end_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_back_end_ret;
    (void)codes_used;
    (void)combined_adler;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}