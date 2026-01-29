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
//<ID> 1278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef src_data[] = "zlib API payload: tune deflate, set gzip header, write via gzopen64 and get 64-bit tell";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    static Bytef name_buf[] = "test_name";
    static Bytef comment_buf[] = "test_comment";
    head.name = name_buf;
    head.name_max = (uInt)(sizeof(name_buf) - 1);
    head.comment = comment_buf;
    head.comm_max = (uInt)(sizeof(comment_buf) - 1);
    head.text = 1;
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure compression and header/tuning
    int hdr_ret = deflateSetHeader(&strm, &head);
    int tune_ret = deflateTune(&strm, 16, 32, 64, 128);
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src_data;
    strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate - deflate, write using gzopen64, obtain 64-bit position
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off64_t gz_pos64 = gztell64(gzf);

    // step 4: Validate (implicit) and cleanup
    int def_end_ret = deflateEnd(&strm);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)hdr_ret;
    (void)tune_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)gz_pos64;
    (void)def_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}