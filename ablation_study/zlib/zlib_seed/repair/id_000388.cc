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
//<ID> 388
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input[] = "zlib test payload for deflate, deflateBound, deflateCopy and gzwrite.";
    uLong input_len = (uLong)(sizeof(input) - 1);
    z_stream src_strm;
    z_stream dst_strm;
    unsigned char outbuf[512];
    gzFile gz_wr;
    int ret_deflate_init;
    int ret_deflate;
    int ret_deflate_copy;
    int ret_deflate_end_dst;
    int ret_deflate_end_src;
    int ret_gzwrite;
    int ret_gzclose;
    uLong bound;
    unsigned int write_len;

    memset(&src_strm, 0, sizeof(src_strm));
    memset(&dst_strm, 0, sizeof(dst_strm));
    memset(outbuf, 0, sizeof(outbuf));

    // step 2: Setup / Configure
    ret_deflate_init = deflateInit_(&src_strm, 6, ver, (int)sizeof(z_stream));
    bound = deflateBound(&src_strm, input_len);
    outbuf[0] = (unsigned char)(bound & 0xFF);
    src_strm.next_in = (Bytef *)input;
    src_strm.avail_in = (uInt)input_len;
    src_strm.next_out = outbuf;
    src_strm.avail_out = (uInt)sizeof(outbuf);

    // step 3: Operate - perform deflate, copy stream state, write compressed output
    ret_deflate = deflate(&src_strm, 4);
    ret_deflate_copy = deflateCopy(&dst_strm, &src_strm);
    write_len = (unsigned int)(src_strm.total_out & 0xFFFFFFFFUL);
    gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gz_wr, (voidpc)outbuf, write_len);
    ret_gzclose = gzclose(gz_wr);

    // step 4: Validate / Cleanup
    ret_deflate_end_dst = deflateEnd(&dst_strm);
    ret_deflate_end_src = deflateEnd(&src_strm);
    (void)ver;
    (void)input_len;
    (void)ret_deflate_init;
    (void)ret_deflate;
    (void)ret_deflate_copy;
    (void)ret_deflate_end_dst;
    (void)ret_deflate_end_src;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    (void)bound;
    (void)write_len;
    // API sequence test completed successfully
    return 66;
}