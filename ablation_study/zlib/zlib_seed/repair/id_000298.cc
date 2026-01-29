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
//<ID> 298
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    z_stream def_strm;
    z_stream inf_strm;
    const char *ver;
    Bytef input_buf[] = "zlib API sequence payload: deflateTune -> deflate -> inflate -> inflateBackEnd -> gz operations with gzseek64.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt input_len_u = (uInt)input_len;
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    uLong comp_len;
    uLong decomp_len;
    int ret_deflate_init;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_deflate_tune;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_backend;
    int ret_inflate_end;
    gzFile gz;
    int bytes_written;
    off64_t seek_pos;

    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();

    // step 2: Initialize deflate, tune parameters, compress data, and finish deflate
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_tune = deflateTune(&def_strm, 4, 8, 16, 32);
    def_strm.next_in = input_buf;
    def_strm.avail_in = input_len_u;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);

    // step 3: Initialize inflate, provide compressed input, inflate, query backend and end inflate
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_backend = inflateBackEnd(&inf_strm);
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Gzip file operations: open, set buffer, write compressed data, seek using gzseek64, close
    gz = gzopen("test_zlib_api_sequence64.gz", "wb+");
    gzbuffer(gz, 8192);
    bytes_written = gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len);
    seek_pos = gzseek64(gz, (off64_t)0, 1);
    gzclose(gz);

    (void)ret_deflate_init;
    (void)ret_deflate_tune;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_backend;
    (void)ret_inflate_end;
    (void)comp_len;
    (void)decomp_len;
    (void)bytes_written;
    (void)seek_pos;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}