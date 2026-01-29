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
//<ID> 1254
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "Example payload for zlib sequence: split for adler combine, compress, write gz, and inflate back";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure compressor, reset, and prepare buffers
    int def_reset_ret = deflateReset(&def_strm);
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate compressor, write to gz with buffering and get offset
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzbuf_ret = gzbuffer(gzf, 8192);
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off_t gz_off = gzoffset(gzf);
    int gzclose_ret1 = gzclose(gzf);

    // step 4: Inflate back, compute adler combination, cleanup and finalization
    Bytef *out_buf = (Bytef *)malloc((size_t)(source_len + 64));
    memset(out_buf, 0, (int)(source_len + 64));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)((source_len + 64) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(source_len + 64));
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    uInt first_len = (uInt)(source_len / 2);
    uInt second_len = (uInt)(source_len - (uLong)first_len);
    uLong ad1 = adler32(0L, source, first_len);
    uLong ad2 = adler32(0L, source + first_len, second_len);
    uLong ad_combined = adler32_combine64(ad1, ad2, (off64_t)second_len);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_reset_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)gzbuf_ret;
    (void)gz_write_ret;
    (void)gz_off;
    (void)gzclose_ret1;
    (void)inf_ret;
    (void)out_size;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}