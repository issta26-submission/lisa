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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    z_stream back_strm;
    gzFile gz;
    const char input_buf[] = "zlib API sequence test payload: write with gzputs, read with gzgets, inspect inflate back state.\n";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    char read_buf[512];
    unsigned char window[32768];
    const char *ver;
    int ret_gz_open_w;
    int ret_gz_puts;
    int ret_gz_close_w;
    int ret_gz_open_r;
    char *ret_gz_gets;
    int ret_gzeof;
    int ret_gz_close_r;
    int ret_inflate_back_init;
    unsigned long codes_used;
    int ret_inflate_back_end;

    // step 2: Setup & Configure
    memset(&back_strm, 0, sizeof(back_strm));
    memset(read_buf, 0, sizeof(read_buf));
    memset(window, 0, sizeof(window));
    ver = zlibVersion();

    // step 3: Operate - write a gzipped file via gzputs, then read it back via gzgets and query EOF
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzbuffer(gz, 16384);
    ret_gz_puts = gzputs(gz, input_buf);
    ret_gz_close_w = gzclose(gz);

    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    gzbuffer(gz, 8192);
    ret_gz_gets = gzgets(gz, read_buf, (int)sizeof(read_buf));
    ret_gzeof = gzeof(gz);
    ret_gz_close_r = gzclose(gz);

    // step 4: Validate & Cleanup - initialize inflate back-end, inspect codes used, then clean up
    ret_inflate_back_init = inflateBackInit_(&back_strm, 15, window, ver, (int)sizeof(z_stream));
    codes_used = inflateCodesUsed(&back_strm);
    ret_inflate_back_end = inflateBackEnd(&back_strm);

    (void)input_len;
    (void)read_buf;
    (void)ret_gz_open_w;
    (void)ret_gz_puts;
    (void)ret_gz_close_w;
    (void)ret_gz_open_r;
    (void)ret_gz_gets;
    (void)ret_gzeof;
    (void)ret_gz_close_r;
    (void)ret_inflate_back_init;
    (void)codes_used;
    (void)ret_inflate_back_end;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}