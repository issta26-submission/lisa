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
//<ID> 1251
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload: compress, combine adler, write gz, then inflate";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int def_reset_ret = deflateReset(&def_strm);

    // step 2: Configure compression buffers and perform deflate
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    uLong a1 = adler32(0L, source, (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len));
    uLong combined_adler = adler32_combine64(a1, a1, (off64_t)comp_size);
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Write compressed data to a gzip file and inspect offsets and buffering
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzbuf_ret = gzbuffer(gzf, 8192);
    off_t offset_before = gzoffset(gzf);
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off_t offset_after = gzoffset(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Initialize inflate, decompress the in-memory compressed buffer, cleanup
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    uLong out_cap = source_len + 256;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_cap);
    memset(out_buf, 0, (int)out_cap);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_cap > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_cap);
    int inf_ret = inflate(&inf_strm, 0);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)combined_adler;
    (void)def_end_ret;
    (void)gzbuf_ret;
    (void)offset_before;
    (void)gz_write_ret;
    (void)offset_after;
    (void)gzclose_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}