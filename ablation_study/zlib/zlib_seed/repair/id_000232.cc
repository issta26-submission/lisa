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
//<ID> 232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API sequence payload: compress then inflate, also exercise gz reads (gzfread, gzgetc, gzgetc_).";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    char readbuf[128];
    const char *ver;
    gzFile gz;
    int ret_compress;
    uLong comp_len;
    uLong dest_len;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    z_size_t items_read;
    int c1;
    int c2;
    int ret_gz_close;

    // step 2: Setup / Initialize
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    memset(readbuf, 0, sizeof(readbuf));
    ver = zlibVersion();
    dest_len = (uLong)sizeof(compbuf);
    ret_compress = compress2(compbuf, &dest_len, input_buf, input_len, 6);
    comp_len = dest_len;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)input_buf);
    ret_gz_close = gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");

    // step 3: Operate / Validate
    items_read = gzfread((voidp)readbuf, (z_size_t)1, (z_size_t)(sizeof(readbuf) - 1), gz);
    c1 = gzgetc(gz);
    c2 = gzgetc_(gz);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Cleanup
    ret_gz_close = gzclose(gz);
    (void)ret_compress;
    (void)comp_len;
    (void)dest_len;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)items_read;
    (void)c1;
    (void)c2;
    (void)ret_gz_close;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}