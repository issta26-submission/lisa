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
//<ID> 264
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
    z_stream back_strm;
    Bytef input_buf[] = "zlib API sequence payload: deflate -> inflate -> gzip text via gzputs/gzgets -> inspect inflate codes.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    unsigned char window[32768];
    char gz_readbuf[256];
    const char *ver;
    gzFile gz;
    int ret_def_init;
    int ret_def_call;
    int ret_def_end;
    int ret_inf_init;
    int ret_inf_call;
    int ret_inf_end;
    int ret_back_init;
    int ret_back_end;
    int ret_gz_puts;
    char *ret_gz_gets;
    int ret_gz_eof;
    uLong comp_len;
    uLong decomp_len;
    unsigned long codes_used;

    // step 2: Setup and initialization
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&back_strm, 0, sizeof(back_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    memset(window, 0, sizeof(window));
    memset(gz_readbuf, 0, sizeof(gz_readbuf));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_back_init = inflateBackInit_(&back_strm, 15, window, ver, (int)sizeof(z_stream));

    // step 3: Core operations (compress -> inflate) and gzip text ops
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_def_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_def_end = deflateEnd(&def_strm);

    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inf_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    codes_used = inflateCodesUsed(&inf_strm);

    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_puts = gzputs(gz, "gzputs: zlib test message\n");
    ret_gz_eof = 0;
    ret_gz_eof = ret_gz_eof; /* keep variable used */
    ret_gz_eof = ret_gz_eof; /* no-op to silence unused warning */
    ret_gz_eof = ret_gz_eof; /* repeated no-op */
    ret_gz_eof = ret_gz_eof;
    ret_def_init = ret_def_init; /* silence unused warnings */
    gzclose(gz);

    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    ret_gz_gets = gzgets(gz, gz_readbuf, (int)sizeof(gz_readbuf));
    ret_gz_eof = gzeof(gz);
    gzclose(gz);

    // step 4: Validation and cleanup
    ret_back_end = inflateBackEnd(&back_strm);
    ret_inf_end = inflateEnd(&inf_strm);

    (void)ret_def_init;
    (void)ret_def_call;
    (void)ret_def_end;
    (void)ret_inf_init;
    (void)ret_inf_call;
    (void)ret_inf_end;
    (void)ret_back_init;
    (void)ret_back_end;
    (void)ret_gz_puts;
    (void)ret_gz_gets;
    (void)ret_gz_eof;
    (void)comp_len;
    (void)decomp_len;
    (void)codes_used;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}