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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream def_src;
    z_stream def_dst;
    z_stream inf_strm;
    const Bytef input_data[] = "Example input data for zlib API sequence test to exercise deflate/inflate and related calls.";
    uInt input_len = (uInt)(sizeof(input_data) - 1);
    Bytef comp_buf[1024];
    Bytef decomp_buf[1024];
    gzFile gzf;
    const char * ver;
    int ret_deflate_init;
    int ret_deflate;
    int ret_deflate_tune;
    int ret_deflate_copy;
    int ret_deflate_end_src;
    int ret_deflate_end_dst;
    int ret_inflate_init2;
    int ret_inflate;
    int ret_inflate_end;
    off_t file_offset;
    unsigned long codes_used;
    uInt compressed_len;
    int ret_gzwrite;

    // step 2: Setup
    memset(&def_src, 0, sizeof(def_src));
    memset(&def_dst, 0, sizeof(def_dst));
    memset(&inf_strm, 0, sizeof(inf_strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_src, 6, ver, (int)sizeof(z_stream));
    def_src.next_in = (Bytef *)input_data;
    def_src.avail_in = input_len;
    def_src.next_out = comp_buf;
    def_src.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Core operations
    ret_deflate = deflate(&def_src, 0);
    compressed_len = (uInt)def_src.total_out;
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf, comp_buf, compressed_len);
    file_offset = gzoffset(gzf);
    ret_deflate_tune = deflateTune(&def_src, 4, 8, 32, 128);
    ret_deflate_copy = deflateCopy(&def_dst, &def_src);

    ret_inflate_init2 = inflateInit2_(&inf_strm, 15, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = compressed_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inf_strm, 0);
    codes_used = inflateCodesUsed(&inf_strm);

    // step 4: Cleanup
    ret_deflate_end_src = deflateEnd(&def_src);
    ret_deflate_end_dst = deflateEnd(&def_dst);
    ret_inflate_end = inflateEnd(&inf_strm);
    gzclose(gzf);

    (void)ver;
    (void)ret_deflate_init;
    (void)ret_deflate;
    (void)compressed_len;
    (void)ret_gzwrite;
    (void)file_offset;
    (void)ret_deflate_tune;
    (void)ret_deflate_copy;
    (void)ret_inflate_init2;
    (void)ret_inflate;
    (void)codes_used;
    (void)ret_deflate_end_src;
    (void)ret_deflate_end_dst;
    (void)ret_inflate_end;

    // API sequence test completed successfully
    return 66;
}