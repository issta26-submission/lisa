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
//<ID> 1175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and deflate stream
    const Bytef source[] = "zlib API sequence payload demonstrating deflate/inflateSetDictionary/adler32/adler32_combine64/deflateEnd usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);

    // step 2: Compress source into comp_buf
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;

    // step 3: Initialize inflate stream, set dictionary, inflate and compute checksums
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 256;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (int)out_alloc);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    int inf_setdict_ret = inflateSetDictionary(&inf_strm, source, (uInt)source_len);
    uLong adler_a = adler32(0UL, source, (uInt)source_len);
    uLong adler_b = adler32(0UL, out_buf, (uInt)(out_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_size));
    uLong combined = adler32_combine64(adler_a, adler_b, (off64_t)out_size);

    // step 4: Cleanup streams and buffers
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(out_buf);

    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_setdict_ret;
    (void)adler_a;
    (void)adler_b;
    (void)combined;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)version;
    (void)source_len;
    (void)out_alloc;
    // API sequence test completed successfully
    return 66;
}