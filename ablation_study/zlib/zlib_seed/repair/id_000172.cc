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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib integration test payload: compress, write via gz, check offset.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    const char * ver;
    int ret_deflate_init;
    int ret_deflate_reset;
    uLong bound;
    Bytef * out_buf = (Bytef *)0;
    int ret_deflate;
    uLong compressed_len;
    gzFile gz = (gzFile)0;
    int ret_gz_write;
    int ret_gz_flush;
    off_t file_offset;
    int ret_deflate_end;

    // step 2: Setup (Initialize)
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, -1, ver, (int)sizeof(z_stream));
    ret_deflate_reset = deflateReset(&strm);

    // step 3: Configure and Operate (Compress then write)
    bound = deflateBound(&strm, input_len);
    out_buf = (Bytef *)malloc((size_t)bound);
    strm.next_in = (Bytef *)input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)bound;
    ret_deflate = deflate(&strm, 4); /* 4 == Z_FINISH */
    compressed_len = strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)out_buf, (unsigned int)compressed_len);
    ret_gz_flush = gzflush(gz, 1);

    // step 4: Validate and Cleanup
    file_offset = gzoffset(gz);
    ret_deflate_end = deflateEnd(&strm);
    gzclose(gz);
    free(out_buf);
    (void)ret_deflate_init;
    (void)ret_deflate_reset;
    (void)ret_deflate;
    (void)ret_gz_write;
    (void)ret_gz_flush;
    (void)ret_deflate_end;
    (void)ver;
    (void)compressed_len;
    (void)file_offset;
    // API sequence test completed successfully
    return 66;
}