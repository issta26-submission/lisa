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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream def_strm;
    z_stream inf_strm;
    Bytef inbuf[] = "Example input data for zlib compression and subsequent inflate via gz file.";
    uInt in_len = (uInt)(sizeof(inbuf) - 1);
    Bytef comp_buf[512];
    Bytef read_buf[512];
    Bytef decomp_buf[512];
    gzFile gzf;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_deflate;
    int ret_inflate;
    int ret_deflate_end;
    int ret_inflate_end;
    int ret_gzbuffer;
    int ret_gzwrite;
    int ret_gzclose;
    int ret_deflate_reset;
    unsigned int compressed_len;
    z_size_t items_read;
    off_t seek_ret;

    // step 2: Setup (initialize streams and open gzip file)
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    ret_deflate_init = deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    ret_inflate_init = inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)inbuf;
    def_strm.avail_in = in_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "w+b");
    ret_gzbuffer = gzbuffer(gzf, 4096);

    // step 3: Core operations (deflate -> write gz -> seek -> read -> inflate -> reset deflate)
    ret_deflate = deflate(&def_strm, 0);
    compressed_len = (unsigned int)def_strm.total_out;
    ret_gzwrite = gzwrite(gzf, comp_buf, compressed_len);
    seek_ret = gzseek(gzf, 0, 0);
    items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)compressed_len, gzf);
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)items_read;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inf_strm, 0);
    ret_deflate_reset = deflateReset(&def_strm);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&def_strm);
    ret_inflate_end = inflateEnd(&inf_strm);
    ret_gzclose = gzclose(gzf);

    (void)ret_deflate_init;
    (void)ret_inflate_init;
    (void)ret_deflate;
    (void)compressed_len;
    (void)ret_gzwrite;
    (void)seek_ret;
    (void)items_read;
    (void)ret_inflate;
    (void)ret_deflate_reset;
    (void)ret_deflate_end;
    (void)ret_inflate_end;
    (void)ret_gzbuffer;
    (void)ret_gzclose;

    // API sequence test completed successfully
    return 66;
}