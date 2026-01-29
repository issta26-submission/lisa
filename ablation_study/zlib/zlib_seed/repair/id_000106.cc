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
//<ID> 106
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib API sequence test input data for compression, CRC, gz write/read and inflate.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[4096];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef decomp_buf[4096];
    gzFile gzf_w;
    gzFile gzf_r;
    const char * ver;
    uLong crc;
    off64_t offset64;
    int eof_flag;
    int ret_compress;
    int ret_gzwrite;
    int ret_gzclose_w;
    int ret_inflate_init;
    int ret_inflate;
    int ret_inflate_back_end;
    int ret_inflate_end;
    int ret_gzclose_r;

    // step 2: Setup (initialize structures and compute CRC)
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    crc = crc32(0UL, input_buf, (uInt)input_len);

    // step 3: Core operations (compress data, write to gz, reopen, check offset/eof, inflate)
    ret_compress = compress(comp_buf, &comp_len, input_buf, input_len);
    gzf_w = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    ret_gzclose_w = gzclose(gzf_w);
    gzf_r = gzopen64("test_zlib_api_sequence.tmp.gz", "rb");
    offset64 = gzoffset64(gzf_r);
    eof_flag = gzeof(gzf_r);
    ret_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)comp_len;
    strm.next_out = decomp_buf;
    strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&strm, 0);

    // step 4: Cleanup (call inflateBackEnd as extra cleanup, end inflate, close gz reader)
    ret_inflate_back_end = inflateBackEnd(&strm);
    ret_inflate_end = inflateEnd(&strm);
    ret_gzclose_r = gzclose_r(gzf_r);

    (void)ret_compress;
    (void)ret_gzwrite;
    (void)ret_gzclose_w;
    (void)ret_inflate_init;
    (void)ret_inflate;
    (void)ret_inflate_back_end;
    (void)ret_inflate_end;
    (void)ret_gzclose_r;
    (void)ver;
    (void)crc;
    (void)offset64;
    (void)eof_flag;
    (void)strm.total_out;
    (void)comp_len;
    (void)input_len;

    // API sequence test completed successfully
    return 66;
}