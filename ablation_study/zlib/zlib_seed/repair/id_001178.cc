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
//<ID> 1178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize data and compute source Adler-32
    const Bytef source[] = "zlib API sequence payload demonstrating deflateEnd/adler32/adler32_combine64/inflateSetDictionary usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong adler_src = adler32(0UL, source, (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len));
    const char *version = zlibVersion();

    // step 2: Initialize deflate stream, compress source into allocated buffer
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;

    // step 3: Initialize inflate stream, set dictionary, compute checksums and combine them
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int inf_setdict_ret = inflateSetDictionary(&inf_strm, source, (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len));
    uLong adler_comp = adler32(0UL, comp_buf, (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size));
    uLong adler_combined = adler32_combine64(adler_src, adler_comp, (off64_t)comp_size);

    // step 4: Cleanup streams and buffers
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);

    (void)def_init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)inf_init_ret;
    (void)inf_setdict_ret;
    (void)adler_src;
    (void)adler_comp;
    (void)adler_combined;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)version;
    (void)bound;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}