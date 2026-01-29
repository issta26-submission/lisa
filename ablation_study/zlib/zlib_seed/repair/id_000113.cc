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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "Example data to compress with zlib for API sequence testing.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[65536];
    const char * ver;
    int ret_init;
    int ret_tune;
    int ret_deflate;
    int ret_end;
    uLong crc;
    uInt comp_buf_size = (uInt)sizeof(comp_buf);

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 3: Core operations (configure tuning, compress and compute CRC)
    ret_tune = deflateTune(&strm, 20, 10, 15, 4);
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = comp_buf;
    strm.avail_out = comp_buf_size;
    ret_deflate = deflate(&strm, 4);
    crc = crc32(0UL, input_buf, (uInt)input_len);

    // step 4: Cleanup
    ret_end = deflateEnd(&strm);
    (void)ret_init;
    (void)ret_tune;
    (void)ret_deflate;
    (void)ret_end;
    (void)ver;
    (void)crc;
    (void)comp_buf_size;
    (void)strm.total_out;

    // API sequence test completed successfully
    return 66;
}