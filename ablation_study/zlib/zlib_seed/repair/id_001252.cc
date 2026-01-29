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
//<ID> 1252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "Example payload for zlib: compress, write gz, reset deflate, combine adler32 parts and inflate back";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure compression buffers and compute bounds
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate: deflate, write to gz with buffer, reset deflate, compute combined adler32, then inflate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_buf_ret = gzbuffer(gzf, 4096);
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off_t gz_off = gzoffset(gzf);
    int gz_close_ret = gzclose(gzf);
    int def_reset_ret = deflateReset(&def_strm);

    uInt half_len = (uInt)((source_len / 2) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(source_len / 2));
    uInt second_len = (uInt)(source_len - (uLong)half_len);
    uLong adler_a = adler32(0L, source, half_len);
    uLong adler_b = adler32(0L, source + half_len, second_len);
    off64_t sec_len64 = (off64_t)second_len;
    uLong combined_adler = adler32_combine64(adler_a, adler_b, sec_len64);

    Bytef *out_buf = (Bytef *)malloc((size_t)(source_len + 16));
    memset(out_buf, 0, (int)(source_len + 16));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)((source_len + 16) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(source_len + 16));
    int inf_ret = inflate(&inf_strm, 0);
    uLong recovered = inf_strm.total_out;

    // step 4: Cleanup and finalization
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gz_buf_ret;
    (void)gz_write_ret;
    (void)gz_off;
    (void)gz_close_ret;
    (void)def_reset_ret;
    (void)adler_a;
    (void)adler_b;
    (void)combined_adler;
    (void)recovered;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}