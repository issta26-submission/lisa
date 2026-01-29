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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream back_strm;
    Bytef input_buf[] = "zlib API sequence test payload for compression, CRC, and inflateBack lifecycle.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[4096];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    const char * ver;
    gzFile gzf_w;
    gzFile gzf_r;
    int ret_compress;
    int ret_gzwrite;
    int ret_gzclose_w;
    off64_t offset64;
    int eof_flag;
    uLong crc;
    int ret_inflateBackInit;
    int ret_inflateBackEnd;
    int ret_gzclose_r;

    // step 2: Setup (initialize version, compress data, write to gz)
    ver = zlibVersion();
    memset(&back_strm, 0, sizeof(back_strm));
    ret_compress = compress2(comp_buf, &comp_len, input_buf, input_len, 6);
    gzf_w = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    ret_gzclose_w = gzclose(gzf_w);

    // step 3: Operate & Validate (open for read, check offset/EOF, compute CRC, initialize inflateBack and end it)
    gzf_r = gzopen64("test_zlib_api_sequence.tmp.gz", "rb");
    offset64 = gzoffset64(gzf_r);
    eof_flag = gzeof(gzf_r);
    crc = crc32(0UL, comp_buf, (uInt)comp_len);
    {
        unsigned char window[32768];
        memset(window, 0, sizeof(window));
        ret_inflateBackInit = inflateBackInit_(&back_strm, 15, window, ver, (int)sizeof(z_stream));
    }
    ret_inflateBackEnd = inflateBackEnd(&back_strm);
    ret_gzclose_r = gzclose_r(gzf_r);

    // step 4: Cleanup (finalize and suppress unused warnings)
    (void)ret_compress;
    (void)ret_gzwrite;
    (void)ret_gzclose_w;
    (void)offset64;
    (void)eof_flag;
    (void)crc;
    (void)ret_inflateBackInit;
    (void)ret_inflateBackEnd;
    (void)ret_gzclose_r;
    (void)ver;
    (void)input_len;
    (void)comp_len;
    (void)back_strm.total_out;

    // API sequence test completed successfully
    return 66;
}