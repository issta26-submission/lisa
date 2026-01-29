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
//<ID> 231
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API sequence payload: compress with compress2, write gz, read gz with gzfread/gzgetc, then inflate.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    Bytef gz_read_buf[65536];
    uLongf comp_len;
    const char *ver;
    gzFile gz;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    z_size_t gz_read_count;
    int c_from_gz_getc_;
    int c_from_gz_getc;

    // step 2: Setup / compress
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    memset(gz_read_buf, 0, sizeof(gz_read_buf));
    ver = zlibVersion();
    comp_len = (uLongf)sizeof(compbuf);
    ret_compress = compress2(compbuf, &comp_len, input_buf, input_len, 6);

    // step 3: Write original data to a gz file, then read it back using gzfread, gzgetc_ and gzgetc
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)input_buf, (unsigned int)input_len);
    gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    gz_read_count = gzfread((voidp)gz_read_buf, (z_size_t)1, (z_size_t)sizeof(gz_read_buf), gz);
    c_from_gz_getc_ = gzgetc_(gz);
    c_from_gz_getc = gzgetc(gz);
    gzclose(gz);

    // step 4: Inflate compressed buffer, validate and cleanup
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    ret_inflate_end = inflateEnd(&inf_strm);

    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)gz_read_count;
    (void)c_from_gz_getc_;
    (void)c_from_gz_getc;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}