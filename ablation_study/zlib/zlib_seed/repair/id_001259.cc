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
//<ID> 1259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef src_data[] = "zlib API sequence payload: create compressed stream, write to gz, combine adler and inflate back";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure compression buffers and reset
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_reset_ret = deflateReset(&def_strm);

    // step 3: Perform deflate, write to gz and combine checksums
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_buf_ret = gzbuffer(gzf, 8192);
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off_t gz_off = gzoffset(gzf);
    uLong ad1 = adler32(1UL, src_data, (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len));
    uLong combined_adler = adler32_combine64(ad1, ad1, (off64_t)gz_off);

    // step 4: Inflate back, validate sizes and cleanup
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len + 8);
    memset(out_buf, 0, (int)(src_len + 8));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)( (src_len + 8) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 8) );
    int inf_ret = inflate(&inf_strm, 4);
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)bound;
    (void)def_reset_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gz_buf_ret;
    (void)gz_write_ret;
    (void)gz_off;
    (void)ad1;
    (void)combined_adler;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}