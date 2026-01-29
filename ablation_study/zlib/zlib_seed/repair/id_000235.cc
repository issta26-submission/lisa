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
//<ID> 235
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API sequence: compress with compress2, inflate the compressed buffer, write/read a gz file, verify reads with gzgetc/gzgetc_.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef inflate_out[65536];
    char readbuf[65536];
    const char *ver;
    gzFile gz;
    uLong bound;
    uLongf comp_len_f;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    z_size_t gzfread_ret;
    int gzgetc_ret;
    int gzgetc__ret;
    int ret_gzwrite;
    int ret_gzclose;

    // step 2: Setup
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(inflate_out, 0, sizeof(inflate_out));
    memset(readbuf, 0, sizeof(readbuf));
    ver = zlibVersion();
    bound = compressBound(input_len);
    comp_len_f = (uLongf)bound;
    ret_compress = compress2(compbuf, &comp_len_f, input_buf, input_len, 6);

    // step 3: Core operations
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len_f;
    inf_strm.next_out = inflate_out;
    inf_strm.avail_out = (uInt)sizeof(inflate_out);
    ret_inflate_call = inflate(&inf_strm, 4);
    ret_inflate_end = inflateEnd(&inf_strm);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzwrite = gzwrite(gz, input_buf, (unsigned int)input_len);
    ret_gzclose = gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    gzfread_ret = gzfread(readbuf, 1, (z_size_t)sizeof(readbuf), gz);
    gzgetc_ret = gzgetc(gz);
    gzgetc__ret = gzgetc_(gz);
    gzclose(gz);

    // step 4: Cleanup and finalization
    (void)ver;
    (void)bound;
    (void)comp_len_f;
    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)gzfread_ret;
    (void)gzgetc_ret;
    (void)gzgetc__ret;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    // API sequence test completed successfully
    return 66;
}