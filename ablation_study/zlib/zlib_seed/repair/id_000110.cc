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
//<ID> 110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib API sequence test input for deflate, tuning and crc32 computation.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef out_buf[4096];
    uInt out_size = (uInt)sizeof(out_buf);
    const char * ver;
    int ret_init;
    int ret_tune;
    int ret_deflate1;
    int ret_deflate2;
    int ret_end;
    uLong checksum;

    // step 2: Setup (initialize stream and obtain version)
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    ret_tune = deflateTune(&strm, 4, 32, 258, 4096);

    // step 3: Operate (feed input, perform deflate in two calls to simulate streaming, compute crc)
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = out_buf;
    strm.avail_out = out_size;
    ret_deflate1 = deflate(&strm, 0);   // Z_NO_FLUSH == 0
    ret_deflate2 = deflate(&strm, 4);   // Z_FINISH == 4
    checksum = crc32(0UL, input_buf, (uInt)input_len);

    // step 4: Validate & Cleanup (use totals and end stream)
    ret_end = deflateEnd(&strm);
    (void)ver;
    (void)ret_init;
    (void)ret_tune;
    (void)ret_deflate1;
    (void)ret_deflate2;
    (void)ret_end;
    (void)checksum;
    (void)strm.total_out;

    // API sequence test completed successfully
    return 66;
}