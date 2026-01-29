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
//<ID> 342
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
    Bytef input_buf[] = "Zlib test payload for compress2 -> inflate sequence and gz file buffering/closing.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[16384];
    Bytef decompbuf[16384];
    uLongf comp_len_f;
    int ret_compress;
    int ret_inf_init;
    int ret_inf_call;
    int ret_inf_end;
    gzFile gz;
    int gzbuffer_ret;
    int gzeof_ret;
    int gzclose_r_ret;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    comp_len_f = (uLongf)sizeof(compbuf);
    ret_compress = compress2(compbuf, &comp_len_f, (const Bytef *)input_buf, (uLong)input_len, 6);

    // step 3: Core operations - inflate compressed data and use gz APIs
    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len_f;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inf_call = inflate(&inf_strm, 4);
    ret_inf_end = inflateEnd(&inf_strm);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzbuffer_ret = gzbuffer(gz, 8192);
    gzwrite(gz, (voidpc)compbuf, (unsigned int)comp_len_f);
    gzeof_ret = gzeof(gz);
    gzclose_r_ret = gzclose_r(gz);

    // step 4: Validation / cleanup
    (void)ver;
    (void)input_len;
    (void)comp_len_f;
    (void)ret_compress;
    (void)ret_inf_init;
    (void)ret_inf_call;
    (void)ret_inf_end;
    (void)gzbuffer_ret;
    (void)gzeof_ret;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}