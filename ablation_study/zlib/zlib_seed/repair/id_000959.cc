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
//<ID> 959
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence: deflate into buffer, query pending, inflate, inspect codes, and combine checksums.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 0);
    unsigned int pending_bits = 0;
    int pending_bits_flag = 0;
    int pend_ret = deflatePending(&def_strm, &pending_bits, &pending_bits_flag);
    uLong comp_size = def_strm.total_out;
    uLong adler_part1 = adler32(0UL, source, (uInt)source_len);
    uLong adler_part2 = adler32(0UL, comp_buf, (uInt)comp_size);
    uLong combined_adler = adler32_combine64(adler_part1, adler_part2, (off64_t)comp_size);

    Bytef *decomp_buf = (Bytef *)malloc((size_t)(source_len + 64));
    memset(decomp_buf, 0, (size_t)(source_len + 64));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(source_len + 64);
    int inf_ret = inflate(&inf_strm, 0);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int inf_back_end_ret = inflateBackEnd(&inf_strm);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)bound;
    (void)def_ret;
    (void)pend_ret;
    (void)pending_bits;
    (void)pending_bits_flag;
    (void)comp_size;
    (void)adler_part1;
    (void)adler_part2;
    (void)combined_adler;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_back_end_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}