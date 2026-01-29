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
//<ID> 294
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
    Bytef input_buf[] = "zlib API sequence payload for deflate -> gzwrite -> seek64 -> inflate -> backend cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt input_len_u = (uInt)input_len;
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    uLong comp_len = 0;
    uLong decomp_len = 0;
    const char *ver;
    gzFile gz;
    off64_t seek_res;
    int ret_deflate_init;
    int ret_deflate_tune;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_backend;
    int ret_inflate_end;
    int ret_gzbuffer;
    int bytes_written;
    int ret_gzclose;

    // step 2: Setup and deflate (initialize -> configure -> operate)
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_tune = deflateTune(&def_strm, 32, 258, 258, 4096);
    def_strm.next_in = input_buf;
    def_strm.avail_in = input_len_u;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);

    // step 3: Gzip file operations and inflate (operate -> validate)
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzbuffer = gzbuffer(gz, 4096);
    bytes_written = gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len);
    seek_res = gzseek64(gz, (off64_t)0, 0);
    ret_gzclose = gzclose(gz);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_backend = inflateBackEnd(&inf_strm);
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Cleanup / finalization (validate results and silence unused warnings)
    (void)ver;
    (void)input_len;
    (void)input_len_u;
    (void)comp_len;
    (void)decomp_len;
    (void)ret_deflate_init;
    (void)ret_deflate_tune;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_backend;
    (void)ret_inflate_end;
    (void)ret_gzbuffer;
    (void)bytes_written;
    (void)ret_gzclose;
    (void)seek_res;
    // API sequence test completed successfully
    return 66;
}