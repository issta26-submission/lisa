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
//<ID> 238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API sequence payload: compress2 -> inflate -> gz write/read -> validate.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef decompbuf[65536];
    char readbuf[256];
    const char *ver;
    uLongf compbuf_len;
    gzFile gz;
    int ret_compress2;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    z_size_t read_items;
    int ch_gzgetc;
    int ch_gzgetc_;

    // step 2: Setup
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    memset(readbuf, 0, sizeof(readbuf));
    ver = zlibVersion();
    compbuf_len = (uLongf)sizeof(compbuf);
    ret_compress2 = compress2(compbuf, &compbuf_len, input_buf, input_len, 6);

    // step 3: Inflate the compressed buffer
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)compbuf_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Write original with gz, then read via gzfread and single-character reads
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)input_buf);
    gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    read_items = gzfread(readbuf, 1, sizeof(readbuf), gz);
    ch_gzgetc = gzgetc(gz);
    ch_gzgetc_ = gzgetc_(gz);
    gzclose(gz);

    (void)ret_compress2;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)read_items;
    (void)ch_gzgetc;
    (void)ch_gzgetc_;
    (void)ver;
    (void)decompbuf;
    (void)compbuf_len;
    // API sequence test completed successfully
    return 66;
}