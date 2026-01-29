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
//<ID> 267
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
    Bytef input_buf[] = "zlib API sequence test payload: deflate -> gzputs/gzwrite -> gzread -> inflate -> inspect codes/backend.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef readbuf[65536];
    Bytef outbuf[65536];
    const char *ver;
    gzFile gz;
    int ret_def_init;
    int ret_def;
    int ret_def_end;
    int ret_gz_puts;
    int ret_gz_write;
    int ret_gz_close;
    int ret_gz_open_r;
    int ret_gz_read;
    int ret_gz_close2;
    int ret_inf_init;
    int ret_inf;
    int ret_inf_back;
    int ret_inf_end;
    uLong comp_len;
    uLong read_len;
    uLong decomp_len;
    unsigned long codes_used;
    int eof_flag;
    uInt header_len;

    // step 2: Setup and compression
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(readbuf, 0, sizeof(readbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    ret_def = deflate(&def_strm, 4);
    comp_len = def_strm.total_out;
    ret_def_end = deflateEnd(&def_strm);

    // step 3: Write using gzputs (a small header) and gzwrite (binary compressed data), then close
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    header_len = (uInt)strlen("ZLIBHDR");
    ret_gz_puts = gzputs(gz, "ZLIBHDR");
    ret_gz_write = gzwrite(gz, compbuf, (unsigned int)comp_len);
    ret_gz_close = gzclose(gz);

    // step 4: Read back, inflate, inspect codes used and call inflateBackEnd, then cleanup
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    ret_gz_read = gzread(gz, readbuf, (unsigned int)(header_len + comp_len));
    read_len = (uLong)(ret_gz_read >= 0 ? ret_gz_read : 0);
    eof_flag = gzeof(gz);
    ret_gz_close2 = gzclose(gz);

    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = readbuf + header_len;
    inf_strm.avail_in = (uInt)(read_len > header_len ? (read_len - header_len) : 0);
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inf = inflate(&inf_strm, 4);
    decomp_len = inf_strm.total_out;
    codes_used = inflateCodesUsed(&inf_strm);
    ret_inf_back = inflateBackEnd(&inf_strm);
    ret_inf_end = inflateEnd(&inf_strm);

    (void)ret_def_init;
    (void)ret_def;
    (void)ret_def_end;
    (void)ret_gz_puts;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)ret_gz_open_r;
    (void)ret_gz_read;
    (void)ret_gz_close2;
    (void)ret_inf_init;
    (void)ret_inf;
    (void)ret_inf_back;
    (void)ret_inf_end;
    (void)comp_len;
    (void)read_len;
    (void)decomp_len;
    (void)codes_used;
    (void)eof_flag;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}