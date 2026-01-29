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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_stream;
    Bytef input_buf[] = "Example payload for zlib API sequencing and CRC validation.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[1024];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef decomp_buf[1024];
    const char *ver;
    gzFile gzf;
    uLong crc_input;
    uLong crc_decomp;
    int ret_compress;
    int ret_inflate_init;
    int ret_inflate;
    int ret_inflate_backend;
    int ret_gzwrite;
    int ret_gzclose;
    int ret_gzclose_r;
    int ret_gzeof;
    off64_t offset;

    // step 2: Setup (initialize stream, get version, compress input)
    memset(&inf_stream, 0, sizeof(inf_stream));
    ver = zlibVersion();
    ret_compress = compress2(comp_buf, &comp_len, input_buf, input_len, 6);

    // step 3: Core operations (write compressed data to gz, reopen for read, inflate, compute CRCs, check offset/eof)
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    ret_gzclose = gzclose(gzf);
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "rb");
    ret_gzeof = gzeof(gzf);
    offset = gzoffset64(gzf);
    ret_inflate_init = inflateInit_(&inf_stream, ver, (int)sizeof(z_stream));
    inf_stream.next_in = comp_buf;
    inf_stream.avail_in = (uInt)comp_len;
    inf_stream.next_out = decomp_buf;
    inf_stream.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inf_stream, 0);
    ret_inflate_backend = inflateBackEnd(&inf_stream);
    crc_input = crc32(0, input_buf, (uInt)input_len);
    crc_decomp = crc32(0, decomp_buf, (uInt)inf_stream.total_out);
    ret_gzclose_r = gzclose_r(gzf);

    // step 4: Cleanup (silence unused results and conclude)
    (void)ver;
    (void)ret_compress;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    (void)ret_gzeof;
    (void)offset;
    (void)ret_inflate_init;
    (void)ret_inflate;
    (void)ret_inflate_backend;
    (void)crc_input;
    (void)crc_decomp;
    (void)ret_gzclose_r;
    (void)comp_len;
    (void)inf_stream.total_out;

    // API sequence test completed successfully
    return 66;
}