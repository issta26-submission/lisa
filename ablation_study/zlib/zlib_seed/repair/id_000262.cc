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
//<ID> 262
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    z_stream back_strm;
    Bytef input[] = "zlib API sequence test payload: compress -> inflate -> inflateBackInit/End -> gzputs/gzeof usage.";
    uLong input_len = (uLong)(sizeof(input) - 1);
    Bytef compbuf[65536];
    uLongf compbuf_len_f = (uLongf)sizeof(compbuf);
    Bytef outbuf[65536];
    Bytef readbuf[65536];
    unsigned char window_buf[32768];
    const char *ver;
    gzFile gz;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    int ret_inflate_back_init;
    int ret_inflate_back_end;
    unsigned long codes_used;
    int ret_gz_write;
    int ret_gz_close;
    int ret_gz_open_read;
    int ret_gz_eof_before;
    int ret_gz_read;
    int ret_gz_eof_after;
    uLong decomp_len;
    uLong comp_len;

    // step 2: Setup and compress the input
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&back_strm, 0, sizeof(back_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    memset(readbuf, 0, sizeof(readbuf));
    memset(window_buf, 0, sizeof(window_buf));
    ver = zlibVersion();
    ret_compress = compress(compbuf, &compbuf_len_f, input, (uLong)input_len);
    comp_len = (uLong)compbuf_len_f;

    // step 3: Inflate the compressed buffer and use inflateBackInit_/inflateBackEnd; query codes used
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    codes_used = inflateCodesUsed(&inf_strm);
    ret_inflate_end = inflateEnd(&inf_strm);
    ret_inflate_back_init = inflateBackInit_(&back_strm, 15, window_buf, ver, (int)sizeof(z_stream));
    ret_inflate_back_end = inflateBackEnd(&back_strm);

    // step 4: Write original input to a gz file using gzputs, then reopen and observe gzeof and read contents; cleanup
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzputs(gz, (const char *)input);
    ret_gz_close = gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    ret_gz_eof_before = gzeof(gz);
    ret_gz_read = gzread(gz, readbuf, (unsigned int)sizeof(readbuf));
    ret_gz_eof_after = gzeof(gz);
    ret_gz_close = gzclose(gz);

    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)ret_inflate_back_init;
    (void)ret_inflate_back_end;
    (void)codes_used;
    (void)decomp_len;
    (void)comp_len;
    (void)ver;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)ret_gz_open_read;
    (void)ret_gz_eof_before;
    (void)ret_gz_read;
    (void)ret_gz_eof_after;
    // API sequence test completed successfully
    return 66;
}