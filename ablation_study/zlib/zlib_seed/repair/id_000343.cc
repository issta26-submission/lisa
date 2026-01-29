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
//<ID> 343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    z_stream inf_strm;
    Bytef input_buf[] = "Zlib API sequence: compress with compress2, inflate with inflateInit_, and write gz file buffer/close checks.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt input_len_u = (uInt)input_len;
    uLong comp_bound;
    Bytef compbuf[65536];
    uLongf comp_len_u;
    Bytef decompbuf[65536];
    uLong decomp_len;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    gzFile gz;
    int gz_buff_ret;
    int gz_write_ret;
    int gz_eof_ret;
    int gz_close_ret;

    // step 2: Setup / initialization
    ver = zlibVersion();
    comp_bound = compressBound(input_len);
    if (comp_bound > (uLong)sizeof(compbuf)) comp_bound = (uLong)sizeof(compbuf);
    comp_len_u = (uLongf)comp_bound;
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));

    // step 3: Core operations - compress, inflate, and gz file operations
    ret_compress = compress2(compbuf, &comp_len_u, input_buf, input_len, 6);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len_u;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gz_buff_ret = gzbuffer(gz, 8192);
    gz_write_ret = gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len_u);
    gz_eof_ret = gzeof(gz);
    gz_close_ret = gzclose_r(gz);

    // step 4: Cleanup / validation
    ret_inflate_end = inflateEnd(&inf_strm);
    (void)ver;
    (void)input_len;
    (void)input_len_u;
    (void)comp_bound;
    (void)comp_len_u;
    (void)decomp_len;
    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)gz_buff_ret;
    (void)gz_write_ret;
    (void)gz_eof_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}