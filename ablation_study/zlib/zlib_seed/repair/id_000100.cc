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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib API sequence test: compress, write gz, reopen, inspect, checksum, and cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[4096];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    unsigned char window[32768];
    const char * ver;
    gzFile gzW;
    gzFile gzR;
    int ret_compress;
    int ret_gzwrite;
    int ret_gzclose_w;
    off64_t offset_after_write;
    int eof_flag;
    uLong checksum;
    int ret_inflateBackInit;
    int ret_inflateBackEnd;
    int ret_gzclose_r;

    // step 2: Setup (initialize stream and obtain version, prepare buffers)
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_compress = compress(comp_buf, &comp_len, input_buf, input_len);
    ret_inflateBackInit = inflateBackInit_(&strm, 15, window, ver, (int)sizeof(z_stream));

    // step 3: Core operations (write compressed data to gz, reopen for reading, inspect, compute CRC, call inflateBackEnd)
    gzW = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzW, comp_buf, (unsigned int)comp_len);
    ret_gzclose_w = gzclose(gzW);
    gzR = gzopen64("test_zlib_api_sequence.tmp.gz", "rb");
    offset_after_write = gzoffset64(gzR);
    eof_flag = gzeof(gzR);
    checksum = crc32(0UL, input_buf, (uInt)input_len);
    ret_inflateBackEnd = inflateBackEnd(&strm);

    // step 4: Cleanup (close read gz file and finalize)
    ret_gzclose_r = gzclose_r(gzR);
    (void)ret_compress;
    (void)comp_len;
    (void)ret_gzwrite;
    (void)ret_gzclose_w;
    (void)offset_after_write;
    (void)eof_flag;
    (void)checksum;
    (void)ret_inflateBackInit;
    (void)ret_inflateBackEnd;
    (void)ret_gzclose_r;
    (void)ver;
    (void)input_len;
    (void)strm.total_out;

    // API sequence test completed successfully
    return 66;
}