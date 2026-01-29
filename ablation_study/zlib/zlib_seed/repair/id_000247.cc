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
//<ID> 247
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
    Bytef input_buf[] = "zlib API sequence payload: compress with deflate, tune deflate, write original via gz, seek/read via gzseek64, then inflate back.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef decompbuf[65536];
    Bytef gz_readbuf[65536];
    const char *ver;
    gzFile gz;
    int ret_deflate_init;
    int ret_deflate_tune;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gzbuffer;
    off64_t ret_gzseek64;
    int ret_gzread;
    int ret_gzclose;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    uLong comp_len;
    uLong decomp_len;

    // step 2: Setup (initialize streams and buffers, initialize deflate and tune)
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    memset(gz_readbuf, 0, sizeof(gz_readbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_tune = deflateTune(&def_strm, 128, 64, 258, 1024);

    // step 3: Core operations (compress input, write original via gz, set buffer and seek/read)
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);

    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzbuffer = gzbuffer(gz, 16384);
    ret_gzread = gzwrite(gz, (voidpc)input_buf, (unsigned int)input_len);
    ret_gzclose = gzclose(gz);

    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    ret_gzseek64 = gzseek64(gz, (off64_t)0, SEEK_SET);
    ret_gzread = gzread(gz, (voidp)gz_readbuf, (unsigned int)sizeof(gz_readbuf));
    ret_gzclose = gzclose(gz);

    // step 4: Inflate compressed buffer, validate and cleanup
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);

    (void)ret_deflate_init;
    (void)ret_deflate_tune;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gzbuffer;
    (void)ret_gzseek64;
    (void)ret_gzread;
    (void)ret_gzclose;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)comp_len;
    (void)decomp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}