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
//<ID> 133
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
    Bytef input_buf[] = "zlib API sequence test payload for deflate -> inflate and gzclearerr.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[1024];
    uInt comp_buf_size = (uInt)sizeof(comp_buf);
    Bytef out_buf[1024];
    uInt out_buf_size = (uInt)sizeof(out_buf);
    const char * ver;
    int ret_deflate_init;
    int ret_inflate_init2;
    int ret_deflate;
    int ret_inflate;
    int ret_deflate_end;
    int ret_inflate_end;
    gzFile gzf;

    // step 2: Setup (initialize streams and obtain version)
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(out_buf, 0, sizeof(out_buf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_inflate_init2 = inflateInit2_(&inf_strm, 15, ver, (int)sizeof(z_stream));

    // step 3: Operate (compress with deflate, then decompress with inflate)
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = comp_buf_size;
    ret_deflate = deflate(&def_strm, 0);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = out_buf_size;
    ret_inflate = inflate(&inf_strm, 0);

    // step 4: Cleanup (end streams, operate on gzFile and clear errors)
    ret_deflate_end = deflateEnd(&def_strm);
    ret_inflate_end = inflateEnd(&inf_strm);
    gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzclearerr(gzf);
    gzclose(gzf);

    (void)ret_deflate_init;
    (void)ret_inflate_init2;
    (void)ret_deflate;
    (void)ret_inflate;
    (void)ret_deflate_end;
    (void)ret_inflate_end;
    (void)ver;
    (void)comp_buf_size;
    (void)out_buf_size;
    (void)def_strm.total_out;
    (void)inf_strm.total_out;

    // API sequence test completed successfully
    return 66;
}