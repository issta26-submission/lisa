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
//<ID> 47
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
    Bytef orig[] = "Example data to exercise zlib APIs in a single straight-line sequence.";
    uInt orig_len = (uInt)(sizeof(orig) - 1);
    Bytef comp_buf[512];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef inf_out[512];
    Bytef gz_read_buf[512];
    gzFile gzf;
    z_size_t gz_items_read;
    off_t seek_zero = (off_t)0;
    int ret_compress;
    int ret_inflate_init;
    int ret_deflate_init;
    int ret_inflate;
    int ret_deflate_reset;
    int ret_deflate_end;
    int ret_inflate_end;
    int ret_gzbuffer;
    int ret_gzclose;

    // step 2: Setup (initialize streams and compress source data into memory)
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    ret_compress = compress2(comp_buf, &comp_len, orig, (uLong)orig_len, 6);
    ret_inflate_init = inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    ret_deflate_init = deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = inf_out;
    inf_strm.avail_out = (uInt)sizeof(inf_out);

    // step 3: Core operations (inflate compressed buffer, reset deflate state, use gzFile APIs)
    ret_inflate = inflate(&inf_strm, 0);
    ret_deflate_reset = deflateReset(&def_strm);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "w+b");
    ret_gzbuffer = gzbuffer(gzf, 1024);
    gzwrite(gzf, orig, orig_len);
    gzseek(gzf, seek_zero, 0);
    gz_items_read = gzfread(gz_read_buf, 1, (z_size_t)sizeof(gz_read_buf), gzf);

    // step 4: Cleanup (end streams and close gz file)
    ret_deflate_end = deflateEnd(&def_strm);
    ret_inflate_end = inflateEnd(&inf_strm);
    ret_gzclose = gzclose(gzf);

    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_deflate_init;
    (void)ret_inflate;
    (void)ret_deflate_reset;
    (void)gz_items_read;
    (void)ret_gzbuffer;
    (void)ret_deflate_end;
    (void)ret_inflate_end;
    (void)ret_gzclose;

    // API sequence test completed successfully
    return 66;
}