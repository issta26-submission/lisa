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
//<ID> 62
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
    z_stream copy_strm;
    Bytef inbuf[] = "The quick brown fox jumps over the lazy dog. zlib API sequence test input.";
    uInt in_len = (uInt)(sizeof(inbuf) - 1);
    Bytef comp_buf[1024];
    Bytef decomp_buf[1024];
    gzFile gzf;
    const char * ver;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_deflate;
    int ret_deflatecopy;
    int ret_deflate_tune;
    int ret_deflate_end_src;
    int ret_deflate_end_dst;
    int ret_inflate_end;
    int ret_inflate;
    off_t offset;
    uLong comp_len;
    unsigned long codes_used;
    int ret_gzclose;

    // step 2: Setup (initialize and configure)
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&copy_strm, 0, sizeof(copy_strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_inflate_init = inflateInit2_(&inf_strm, 15, ver, (int)sizeof(z_stream));
    def_strm.next_in = inbuf;
    def_strm.avail_in = in_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    ret_deflate_tune = deflateTune(&def_strm, 4, 8, 16, 32);

    // step 3: Operate (compress, write, copy state, decompress)
    ret_deflate = deflate(&def_strm, 0);
    comp_len = def_strm.total_out;
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    offset = gzoffset(gzf);
    ret_deflatecopy = deflateCopy(&copy_strm, &def_strm);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inf_strm, 0);
    codes_used = inflateCodesUsed(&inf_strm);

    // step 4: Cleanup (validate and free resources)
    ret_deflate_end_src = deflateEnd(&def_strm);
    ret_deflate_end_dst = deflateEnd(&copy_strm);
    ret_inflate_end = inflateEnd(&inf_strm);
    ret_gzclose = gzclose(gzf);

    (void)ver;
    (void)ret_deflate_init;
    (void)ret_inflate_init;
    (void)ret_deflate;
    (void)ret_deflatecopy;
    (void)ret_deflate_tune;
    (void)comp_len;
    (void)offset;
    (void)ret_inflate;
    (void)codes_used;
    (void)ret_deflate_end_src;
    (void)ret_deflate_end_dst;
    (void)ret_inflate_end;
    (void)ret_gzclose;

    // API sequence test completed successfully
    return 66;
}