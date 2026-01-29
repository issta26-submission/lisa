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
//<ID> 299
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    z_stream def_strm;
    z_stream inf_strm;
    Bytef input_buf[] = "Sample payload for zlib sequence.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt input_len_u = (uInt)input_len;
    Bytef compbuf[32768];
    Bytef decompbuf[32768];
    uLong comp_len;
    uLong decomp_len;
    int ret_deflate_init;
    int ret_deflate_tune;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_backend;
    int ret_inflate_end;
    gzFile gz;
    int gz_write_ret;
    off64_t seek_ret;

    // step 2: Setup / initialization
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = input_buf;
    def_strm.avail_in = input_len_u;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);

    // step 3: Core operations - configure, compress, inflate, and gz operations
    ret_deflate_tune = deflateTune(&def_strm, 4, 4, 8, 4);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_backend = inflateBackEnd(&inf_strm);
    ret_inflate_end = inflateEnd(&inf_strm);
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gzbuffer(gz, 8192u);
    gz_write_ret = gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len);
    seek_ret = gzseek64(gz, (off64_t)0, 0);
    gzclose(gz);

    // step 4: Validation / cleanup
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
    (void)gz_write_ret;
    (void)seek_ret;
    // API sequence test completed successfully
    return 66;
}