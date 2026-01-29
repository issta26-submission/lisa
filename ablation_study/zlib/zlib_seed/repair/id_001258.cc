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
//<ID> 1258
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload: split checksum, deflate/reset, write gz, then inflate to verify";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure compression, use deflateReset and compute combined adler32 via adler32_combine64
    int def_reset_ret = deflateReset(&def_strm);
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    uLong first_len = source_len / 2;
    uLong second_len = source_len - first_len;
    uLong adler1 = adler32(0L, source, (uInt)first_len);
    uLong adler2 = adler32(0L, source + first_len, (uInt)second_len);
    uLong combined_adler = adler32_combine64(adler1, adler2, (off64_t)second_len);

    // step 3: Write compressed data to a gzip file, set buffer, query offset, then prepare inflate
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_buf_ret = gzbuffer(gzf, 8192);
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off_t gz_pos = gzoffset(gzf);
    int gz_close_ret = gzclose(gzf);

    Bytef *out_buf = (Bytef *)malloc((size_t)(source_len + 64));
    memset(out_buf, 0, (int)(source_len + 64));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)((source_len + 64) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(source_len + 64));
    int inf_ret = inflate(&inf_strm, 0);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Cleanup and finalization
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_reset_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)bound;
    (void)comp_size;
    (void)adler1;
    (void)adler2;
    (void)combined_adler;
    (void)gz_buf_ret;
    (void)gz_write_ret;
    (void)gz_pos;
    (void)gz_close_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}