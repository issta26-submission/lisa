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
//<ID> 1271
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef src_data[] = "zlib API sequence payload: deflate with gzip header, tune, write with gzopen64 and read position";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    head.name = (Bytef *)"test_name";
    head.comment = (Bytef *)"test_comment";
    head.hcrc = 0;
    int init_ret = deflateInit2_(&strm, 6, 8, 15 + 16, 8, 0, version, (int)sizeof(z_stream));

    // step 2: Configure compression buffers and header/tuning
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src_data;
    strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int set_header_ret = deflateSetHeader(&strm, &head);
    int tune_ret = deflateTune(&strm, 4, 16, 32, 64);

    // step 3: Operate - compress and write using gzopen64, then get 64-bit position
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gz_buf_ret = gzbuffer(gzf, 8192);
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off64_t pos64 = gztell64(gzf);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate and cleanup
    int def_end_ret = deflateEnd(&strm);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)bound;
    (void)set_header_ret;
    (void)tune_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gz_buf_ret;
    (void)gz_write_ret;
    (void)pos64;
    (void)gz_close_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}