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
//<ID> 955
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence payload for deflate/inflate, pending bits, back-end and checksum combine.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* Z_FINISH == 4 */
    unsigned int pending = 0;
    int bits = 0;
    int pend_ret = deflatePending(&def_strm, &pending, &bits);
    uInt produced_len = (uInt)def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 4: Validate / Cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = produced_len;
    uLong decomp_cap = source_len + 64;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_cap);
    memset(decomp_buf, 0, (size_t)decomp_cap);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)decomp_cap;
    int inf_ret = inflate(&inf_strm, 0);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);

    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    unsigned char window[64];
    memset(window, 0, sizeof(window));
    int back_init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int back_end_ret = inflateBackEnd(&back_strm);

    uLong combined_adler = adler32_combine64(inf_strm.adler, /* adler1 */ def_strm.adler, /* adler2 */ (off64_t)1);

    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)pend_ret;
    (void)pending;
    (void)bits;
    (void)produced_len;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)combined_adler;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}