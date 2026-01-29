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
//<ID> 1253
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API payload: compress, reset, write to gz, inflate and validate with adler32_combine64";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure compression, buffers and gz file
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int reset_ret = deflateReset(&def_strm);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int buf_ret = gzbuffer(gzf, 8192);

    // step 3: Operate - deflate, write gz, get offset, then inflate from compressed buffer
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off_t gzf_off = gzoffset(gzf);
    uLong decomp_size_est = source_len + 64;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_size_est);
    memset(decomp_buf, 0, (int)decomp_size_est);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(decomp_size_est > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)decomp_size_est);
    int inf_ret = inflate(&inf_strm, 4);

    // step 4: Validate and cleanup
    uInt decomp_len_u = (uInt)(inf_strm.total_out > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)inf_strm.total_out);
    uInt comp_len_u = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    uLong ad1 = adler32(0UL, decomp_buf, decomp_len_u);
    uLong ad2 = adler32(0UL, comp_buf, comp_len_u);
    uLong combined = adler32_combine64(ad1, ad2, (off64_t)comp_size);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)reset_ret;
    (void)buf_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)gzf_off;
    (void)inf_ret;
    (void)ad1;
    (void)ad2;
    (void)combined;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}