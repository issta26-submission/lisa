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
//<ID> 265
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
    Bytef input_buf[] = "zlib API sequence test payload: use deflate -> write marker with gzputs -> read gzeof -> inflate and inspect codes -> cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    gzFile gz;
    int ret_deflate_init;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_open_write;
    int ret_gz_puts;
    int ret_gz_close_write;
    int ret_gz_open_read;
    int ret_gz_eof;
    int ret_gz_close_read;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    int ret_inflate_back_end;
    unsigned long codes_used;
    uLong comp_len;
    uLong decomp_len;

    // step 2: Setup and initialize deflate
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 3: Compress input and write a textual marker to a gzip file using gzputs
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_deflate_call = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_deflate_end = deflateEnd(&def_strm);

    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_puts = gzputs(gz, "marker: test_zlib_api_sequence\n");
    ret_gz_close_write = gzclose(gz);

    // step 4: Read back gz file to exercise gzeof and then inflate the previously produced compressed buffer,
    // inspect codes used and perform inflateBackEnd and cleanup
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    ret_gz_eof = gzeof(gz);
    ret_gz_close_read = gzclose(gz);

    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    codes_used = inflateCodesUsed(&inf_strm);
    ret_inflate_back_end = inflateBackEnd(&inf_strm);
    ret_inflate_end = inflateEnd(&inf_strm);

    (void)ret_deflate_init;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_puts;
    (void)ret_gz_close_write;
    (void)ret_gz_eof;
    (void)ret_gz_close_read;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_back_end;
    (void)ret_inflate_end;
    (void)comp_len;
    (void)decomp_len;
    (void)codes_used;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}