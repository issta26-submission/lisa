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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API minimal payload: compress then inflate, also write/read gz.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    uLongf comp_len_f;
    uLong comp_len;
    Bytef decompbuf[65536];
    const char *ver;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    gzFile gzw;
    gzFile gzr;
    z_size_t gzfread_ret;
    int ch_gzgetc;
    int ch_gzgetc_;

    // step 2: Setup
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(decompbuf, 0, sizeof(decompbuf));
    ver = zlibVersion();
    comp_len_f = (uLongf)sizeof(compbuf);
    ret_compress = compress2(compbuf, &comp_len_f, input_buf, input_len, 6);
    comp_len = (uLong)comp_len_f;

    // step 3: Core operations
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    ret_inflate_call = inflate(&inf_strm, 4);

    gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzw, (voidpc)input_buf, (unsigned int)input_len);
    gzclose(gzw);

    gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzfread_ret = gzfread(decompbuf, 1, (z_size_t)input_len, gzr);
    ch_gzgetc = gzgetc(gzr);
    ch_gzgetc_ = gzgetc_(gzr);
    gzclose(gzr);

    // step 4: Cleanup and validation
    ret_inflate_end = inflateEnd(&inf_strm);
    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)comp_len;
    (void)gzfread_ret;
    (void)ch_gzgetc;
    (void)ch_gzgetc_;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}