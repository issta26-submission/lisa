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
//<ID> 382
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input_buf[] = "Zlib API sequence: compress this payload, copy stream, write compressed bytes with gzwrite.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    z_stream strm;
    z_stream copy_strm;
    unsigned char out_buf[4096];
    uLong bound;
    uLong compressed_len;
    int def_init_ret;
    int def_ret;
    int def_copy_ret;
    int def_end_ret_orig;
    int def_end_ret_copy;
    gzFile gz_wr;
    int gzwrite_ret;
    int gzclose_ret;

    // step 2: Setup / Configure
    memset(&strm, 0, sizeof(strm));
    memset(&copy_strm, 0, sizeof(copy_strm));
    def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    bound = deflateBound(&strm, input_len);

    // step 3: Operate - compress, copy stream, write compressed output
    strm.next_in = (Bytef *)input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)sizeof(out_buf);
    def_ret = deflate(&strm, 4); // 4 == Z_FINISH
    compressed_len = strm.total_out;
    def_copy_ret = deflateCopy(&copy_strm, &strm);
    gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite_ret = gzwrite(gz_wr, (voidpc)out_buf, (unsigned int)compressed_len);
    gzclose_ret = gzclose(gz_wr);

    // step 4: Validate / Cleanup
    def_end_ret_orig = deflateEnd(&strm);
    def_end_ret_copy = deflateEnd(&copy_strm);
    (void)ver;
    (void)bound;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_copy_ret;
    (void)def_end_ret_orig;
    (void)def_end_ret_copy;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}