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
//<ID> 1257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API flow: initialize, reset, compress, write gz, combine adler, decompress and cleanup";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int def_reset_ret = deflateReset(&def_strm);

    // step 2: Configure compression buffers and prepare streams
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate — compress, combine checksums, write to gz, record offset, then inflate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    uLong half_len = source_len / 2;
    uLong rem_len = source_len - half_len;
    uLong ad1 = adler32(1UL, source, (uInt)(half_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)half_len));
    uLong ad2 = adler32(1UL, source + half_len, (uInt)(rem_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)rem_len));
    uLong combined_adler = adler32_combine64(ad1, ad2, (off64_t)rem_len);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzbuf_ret = gzbuffer(gzf, 4096);
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off_t current_offset = gzoffset(gzf);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(source_len + 16));
    memset(decomp_buf, 0, (int)(source_len + 16));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)((source_len + 16) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(source_len + 16));
    int inf_ret = inflate(&inf_strm, 0);

    // step 4: Validate and cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_reset_ret;
    (void)def_ret;
    (void)comp_size;
    (void)combined_adler;
    (void)gzbuf_ret;
    (void)gz_write_ret;
    (void)current_offset;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}