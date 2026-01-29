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
//<ID> 346
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
    Bytef input_buf[] = "Zlib API sequence test payload to compress, write to gz and inflate back.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uLong comp_bound;
    uLongf comp_len_f;
    Bytef compbuf[16384];
    Bytef decompbuf[16384];
    gzFile gz;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    int ret_gzbuffer;
    int ret_gzwrite;
    int ret_gzeof;
    int ret_gzclose_r;
    uLong decomp_len;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    comp_bound = compressBound(input_len);
    comp_len_f = (uLongf)comp_bound;
    ret_compress = compress(compbuf, &comp_len_f, (const Bytef *)input_buf, input_len);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzbuffer = gzbuffer(gz, 8192);

    // step 3: Core operations - write compressed data, initialize inflate, perform inflate
    ret_gzwrite = gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len_f);
    ret_gzeof = gzeof(gz);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len_f;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Cleanup / validation
    ret_gzclose_r = gzclose_r(gz);
    (void)ver;
    (void)input_len;
    (void)comp_bound;
    (void)comp_len_f;
    (void)ret_compress;
    (void)ret_gzbuffer;
    (void)ret_gzwrite;
    (void)ret_gzeof;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)ret_gzclose_r;
    (void)decomp_len;
    // API sequence test completed successfully
    return 66;
}