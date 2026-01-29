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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and basic initialization
    const char *ver = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API fuzz payload: compress then decompress using deflate/inflate and gz I/O.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt input_len_u = (uInt)input_len;
    Bytef compbuf[16384];
    Bytef outbuf[16384];
    gzFile gz;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    gz = gzopen64("test_zlib_api_sequence.gz", "wb+");
    int ret_gzbuffer = gzbuffer(gz, 8192);

    // step 2: Initialize and tune deflate, then perform a single deflate call
    int ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int ret_deflate_tune = deflateTune(&def_strm, 32, 258, 128, 4096);
    def_strm.next_in = input_buf;
    def_strm.avail_in = input_len_u;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    int ret_deflate_call = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int ret_deflate_end = deflateEnd(&def_strm);

    // step 3: Initialize inflate, perform inflate, then call inflateBackEnd and inflateEnd
    int ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    int ret_inflate_call = inflate(&inf_strm, 4);
    uLong decomp_len = inf_strm.total_out;
    int ret_inflate_backend = inflateBackEnd(&inf_strm);
    int ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Write compressed data to gz file, seek using gzseek64, then close
    int bytes_written = gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len);
    off64_t pos_after_seek = gzseek64(gz, (off64_t)0, 0);
    int ret_gzclose = gzclose(gz);

    (void)ver;
    (void)ret_gzbuffer;
    (void)ret_deflate_init;
    (void)ret_deflate_tune;
    (void)ret_deflate_call;
    (void)comp_len;
    (void)ret_deflate_end;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)decomp_len;
    (void)ret_inflate_backend;
    (void)ret_inflate_end;
    (void)bytes_written;
    (void)pos_after_seek;
    (void)ret_gzclose;
    // API sequence test completed successfully
    return 66;
}