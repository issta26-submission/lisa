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
//<ID> 241
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
    Bytef input_buf[] = "zlib API sequence test payload: compress with deflate, tune compressor, write via gz, seek, then inflate back.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef decompbuf[65536];
    const char *ver;
    gzFile gzf;
    int ret_def_init;
    int ret_def_tune;
    int ret_def_call;
    int ret_def_end;
    int ret_inf_init;
    int ret_inf_call;
    int ret_inf_end;
    int ret_gzbuffer;
    off64_t seek_pos;
    uLong comp_len;
    uLong decomp_len;

    // step 2: Setup and initialize deflate with explicit version/size
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_def_tune = deflateTune(&def_strm, 32, 64, 128, 1024);

    // step 3: Compress input with deflate, finalize, then write original via gz and seek with gzseek64
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_def_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_def_end = deflateEnd(&def_strm);
    gzf = gzopen("test_zlib_api_sequence.gz", "wb+");
    ret_gzbuffer = gzbuffer(gzf, 8192);
    gzwrite(gzf, (voidpc)input_buf, (unsigned int)input_len);
    seek_pos = gzseek64(gzf, (off64_t)0, 0);
    gzclose(gzf);

    // step 4: Initialize inflate, decompress the compressed buffer, cleanup
    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inf_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inf_end = inflateEnd(&inf_strm);

    (void)ret_def_init;
    (void)ret_def_tune;
    (void)ret_def_call;
    (void)ret_def_end;
    (void)ret_inf_init;
    (void)ret_inf_call;
    (void)ret_inf_end;
    (void)ret_gzbuffer;
    (void)seek_pos;
    (void)comp_len;
    (void)decomp_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}