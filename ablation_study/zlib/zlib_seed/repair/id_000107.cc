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
//<ID> 107
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API sequencing test data: ensure meaningful data flow between calls.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[2048];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef comp_buf_read[2048];
    Bytef decomp_buf[2048];
    const char * ver;
    gzFile gzf_w;
    gzFile gzf_r;
    uLong crc;
    off64_t offset64_val;
    int ret_compress;
    int ret_gzwrite;
    int ret_gzclose_w;
    int ret_gzeof;
    int ret_gzread;
    int ret_inflate_init;
    int ret_inflate;
    int ret_inflate_back;
    int ret_inflate_end;
    int ret_gzclose_r;

    // step 2: Setup
    memset(&inf_strm, 0, sizeof(inf_strm));
    ver = zlibVersion();
    ret_compress = compress2(comp_buf, &comp_len, input_buf, input_len, 6);
    crc = crc32(0UL, input_buf, (uInt)input_len);

    // step 3: Core operations (write compressed data to a gz file, read it back, inflate, and use backend)
    gzf_w = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    ret_gzclose_w = gzclose(gzf_w);
    gzf_r = gzopen64("test_zlib_api_sequence.tmp.gz", "rb");
    ret_gzeof = gzeof(gzf_r);
    offset64_val = gzoffset64(gzf_r);
    ret_gzread = gzread(gzf_r, comp_buf_read, (unsigned int)comp_len);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf_read;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inf_strm, 0);
    ret_inflate_back = inflateBackEnd(&inf_strm);
    ret_inflate_end = inflateEnd(&inf_strm);
    ret_gzclose_r = gzclose_r(gzf_r);

    (void)ret_compress;
    (void)crc;
    (void)ret_gzwrite;
    (void)ret_gzclose_w;
    (void)ret_gzeof;
    (void)offset64_val;
    (void)ret_gzread;
    (void)ret_inflate_init;
    (void)ret_inflate;
    (void)ret_inflate_back;
    (void)ret_inflate_end;
    (void)ret_gzclose_r;
    (void)ver;
    (void)input_len;
    (void)comp_len;
    (void)inf_strm.total_out;

    // API sequence test completed successfully
    return 66;
}