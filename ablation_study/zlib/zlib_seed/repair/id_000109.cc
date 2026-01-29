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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream back_strm;
    Bytef input_buf[] = "zlib API sequence test data for compress/write/read/cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[4096];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    unsigned char window[32768];
    const char * ver;
    gzFile gzf_write;
    gzFile gzf_read;
    int ret_inflate_back_init;
    int ret_inflate_back_end;
    int ret_compress;
    int ret_gzwrite;
    int ret_gzclose_write;
    off64_t read_offset;
    int read_eof;
    int ret_gzclose_read;
    uLong crc;

    // step 2: Setup
    memset(&back_strm, 0, sizeof(back_strm));
    memset(window, 0, sizeof(window));
    ver = zlibVersion();

    // step 3: Core operations
    ret_inflate_back_init = inflateBackInit_(&back_strm, 15, window, ver, (int)sizeof(z_stream));
    ret_compress = compress2(comp_buf, &comp_len, input_buf, input_len, 6);
    gzf_write = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf_write, comp_buf, (unsigned int)comp_len);
    ret_gzclose_write = gzclose(gzf_write);
    gzf_read = gzopen64("test_zlib_api_sequence.tmp.gz", "rb");
    read_offset = gzoffset64(gzf_read);
    read_eof = gzeof(gzf_read);
    ret_gzclose_read = gzclose_r(gzf_read);
    crc = crc32(0UL, input_buf, (uInt)input_len);
    ret_inflate_back_end = inflateBackEnd(&back_strm);

    // step 4: Cleanup
    (void)ret_inflate_back_init;
    (void)ret_inflate_back_end;
    (void)ret_compress;
    (void)ret_gzwrite;
    (void)ret_gzclose_write;
    (void)read_offset;
    (void)read_eof;
    (void)ret_gzclose_read;
    (void)crc;
    (void)ver;
    (void)comp_len;
    (void)back_strm.total_out;

    // API sequence test completed successfully
    return 66;
}