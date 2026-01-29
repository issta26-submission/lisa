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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defl_src;
    z_stream defl_copy;
    z_stream infl_strm;
    Bytef inbuf[] = "Example data for zlib API sequence testing: compress, copy state, and inflate.";
    uInt in_len = (uInt)(sizeof(inbuf) - 1);
    Bytef comp_buf[1024];
    Bytef decomp_buf[1024];
    gzFile gzf;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_deflate;
    int ret_inflate;
    int ret_deflate_end_src;
    int ret_deflate_end_copy;
    int ret_inflate_end;
    int ret_deflatecopy;
    int ret_deflatetune;
    off_t gz_off;
    unsigned long codes_used;
    const char *ver;

    // step 2: Setup (initialize streams and version)
    memset(&defl_src, 0, sizeof(defl_src));
    memset(&defl_copy, 0, sizeof(defl_copy));
    memset(&infl_strm, 0, sizeof(infl_strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&defl_src, 6, ver, (int)sizeof(z_stream));
    ret_inflate_init = inflateInit2_(&infl_strm, 15, ver, (int)sizeof(z_stream));
    defl_src.next_in = inbuf;
    defl_src.avail_in = in_len;
    defl_src.next_out = comp_buf;
    defl_src.avail_out = (uInt)sizeof(comp_buf);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Core operations (compress, write, tune, copy, inflate, validate)
    ret_deflate = deflate(&defl_src, 0);
    int bytes_written = gzwrite(gzf, comp_buf, (unsigned int)defl_src.total_out);
    gz_off = gzoffset(gzf);
    ret_deflatetune = deflateTune(&defl_src, 4, 16, 32, 64);
    ret_deflatecopy = deflateCopy(&defl_copy, &defl_src);
    infl_strm.next_in = comp_buf;
    infl_strm.avail_in = (uInt)defl_src.total_out;
    infl_strm.next_out = decomp_buf;
    infl_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&infl_strm, 0);
    codes_used = inflateCodesUsed(&infl_strm);

    // step 4: Cleanup (end streams and close file)
    ret_deflate_end_src = deflateEnd(&defl_src);
    ret_deflate_end_copy = deflateEnd(&defl_copy);
    ret_inflate_end = inflateEnd(&infl_strm);
    gzclose(gzf);
    (void)ver;
    (void)ret_deflate_init;
    (void)ret_inflate_init;
    (void)ret_deflate;
    (void)bytes_written;
    (void)gz_off;
    (void)ret_deflatetune;
    (void)ret_deflatecopy;
    (void)ret_inflate;
    (void)codes_used;
    (void)ret_deflate_end_src;
    (void)ret_deflate_end_copy;
    (void)ret_inflate_end;

    // API sequence test completed successfully
    return 66;
}