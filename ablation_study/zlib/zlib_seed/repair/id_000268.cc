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
//<ID> 268
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef src[] = "zlib API sequence test payload for compress/inflate and gz operations.";
    uLong src_len = (uLong)(sizeof(src) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    uLongf comp_len_f;
    const char *ver;
    gzFile gz;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    unsigned long codes_used;
    int ret_inflate_back;
    int ret_inflate_end;
    int ret_gz_puts;
    int ret_gz_close;
    int ret_gzeof;
    uLong out_len;

    // step 2: Setup and compression
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    comp_len_f = (uLongf)sizeof(compbuf);
    ret_compress = compress2(compbuf, &comp_len_f, src, src_len, 6);

    // step 3: Inflate, query usage and backend cleanup
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len_f;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    out_len = inf_strm.total_out;
    codes_used = inflateCodesUsed(&inf_strm);
    ret_inflate_back = inflateBackEnd(&inf_strm);
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: gz write/read using gzputs and gzeof, cleanup
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_puts = gzputs(gz, "zlib gzputs test line\n");
    ret_gz_close = gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    ret_gzeof = gzeof(gz);
    ret_gz_close = gzclose(gz);

    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)codes_used;
    (void)ret_inflate_back;
    (void)ret_inflate_end;
    (void)ret_gz_puts;
    (void)ret_gz_close;
    (void)ret_gzeof;
    (void)out_len;
    (void)ver;
    (void)src_len;
    (void)comp_len_f;
    // API sequence test completed successfully
    return 66;
}