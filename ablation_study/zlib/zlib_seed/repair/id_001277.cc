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
//<ID> 1277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef src_data[] = "zlib API sequence payload: initialize deflate with gzip header, tune, deflate, write with gzopen64 and validate by inflate";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    gz_header hdr;
    memset(&hdr, 0, (int)sizeof(hdr));
    hdr.text = 0;
    hdr.name = (Bytef *)"test_name";
    hdr.name_max = (uInt)strlen((const char *)hdr.name);

    int def_init_ret = deflateInit2_(&def_strm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    int tune_ret = deflateTune(&def_strm, 4, 4, 8, 16);
    int set_hdr_ret = deflateSetHeader(&def_strm, &hdr);

    // step 2: Configure compression buffers
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate and Validate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off64_t gz_pos = gztell64(gzf);

    int inf_init_ret = inflateInit2_(&inf_strm, 31, version, (int)sizeof(z_stream));
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 8));
    memset(out_buf, 0, (int)(src_len + 8));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)((src_len + 8) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 8));
    int inf_ret = inflate(&inf_strm, 4);

    // step 4: Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)tune_ret;
    (void)set_hdr_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)gz_pos;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}