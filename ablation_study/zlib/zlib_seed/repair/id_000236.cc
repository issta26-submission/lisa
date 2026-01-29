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
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API sequence test payload: compress with compress2, write/read gz, then inflate compressed blob.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef compbuf[65536];
    Bytef outbuf[65536];
    char readbuf[256];
    const char *ver;
    gzFile gz_write = (gzFile)0;
    gzFile gz_read = (gzFile)0;
    uLongf comp_len_f = (uLongf)sizeof(compbuf);
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate_call;
    int ret_inflate_end;
    z_size_t gz_read_count;
    int ch_gzgetc;
    int ch_gzgetc_;
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(compbuf, 0, sizeof(compbuf));
    memset(outbuf, 0, sizeof(outbuf));
    memset(readbuf, 0, sizeof(readbuf));

    // step 2: Setup / Configure
    ver = zlibVersion();
    ret_compress = compress2(compbuf, &comp_len_f, input_buf, input_len, 6);

    // step 3: Operate with gz APIs: write original payload, then read it back with gzfread and single-char reads
    gz_write = gzopen("test_zlib_api_sequence.gz", "wb");
    (void)gzwrite(gz_write, (voidpc)input_buf, (unsigned int)input_len);
    (void)gzclose(gz_write);
    gz_read = gzopen("test_zlib_api_sequence.gz", "rb");
    gz_read_count = gzfread((voidp)readbuf, (z_size_t)1, (z_size_t)sizeof(readbuf), gz_read);
    ch_gzgetc = gzgetc(gz_read);
    ch_gzgetc_ = gzgetc_(gz_read);
    (void)gzclose(gz_read);

    // step 4: Inflate compressed buffer, validate by driving inflate, then cleanup
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = (uInt)comp_len_f;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);
    ret_inflate_call = inflate(&inf_strm, 4);
    ret_inflate_end = inflateEnd(&inf_strm);
    (void)ret_compress;
    (void)ret_inflate_init;
    (void)ret_inflate_call;
    (void)ret_inflate_end;
    (void)gz_read_count;
    (void)ch_gzgetc;
    (void)ch_gzgetc_;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}