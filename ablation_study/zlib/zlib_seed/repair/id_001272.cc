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
//<ID> 1272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef src_data[] = "zlib API sequence payload: compress with tuned deflate, attach header, write via gzopen64 and get position";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configuration (header and tuning)
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    head.text = 0;
    head.time = 0UL;
    head.xflags = 0;
    head.os = 255;
    head.name = (Bytef *)"test_zlib_api_sequence";
    head.name_max = (uInt)(sizeof("test_zlib_api_sequence") - 1);
    int set_head_ret = deflateSetHeader(&def_strm, &head);
    int tune_ret = deflateTune(&def_strm, 32, 16, 64, 4096);

    // step 3: Operate - compress and write via gzopen64, then get position with gztell64
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off64_t gz_pos = gztell64(gzf);

    // step 4: Validate (implicit) and cleanup
    uLong a1 = adler32(1UL, src_data, (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len));
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)set_head_ret;
    (void)tune_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)gz_pos;
    (void)a1;
    (void)def_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}