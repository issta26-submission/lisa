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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    const char *ver;
    Bytef input_buf[] = "zlib: test payload for deflate, deflateBound, deflateReset and gzoffset usage.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uLong bound;
    Bytef *out_buf = (Bytef *)0;
    uLong compressed_len = 0;
    int ret_deflate_init;
    int ret_deflate_reset;
    int ret_deflate_call;
    int ret_deflate_end;
    gzFile gz = (gzFile)0;
    int ret_gz_write;
    int ret_gz_close;
    off_t file_offset;

    // step 2: Setup (initialize z_stream and deflate)
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_reset = deflateReset(&strm);
    bound = deflateBound(&strm, input_len);
    out_buf = (Bytef *)malloc((size_t)bound);
    strm.next_in = (Bytef *)input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate (perform deflate and write original data to gz file, then query offset)
    ret_deflate_call = deflate(&strm, 4); /* 4 == Z_FINISH */
    compressed_len = bound - (uLong)strm.avail_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)input_buf, (unsigned int)input_len);
    file_offset = gzoffset(gz);
    ret_gz_close = gzclose(gz);

    // step 4: Cleanup and validate
    ret_deflate_end = deflateEnd(&strm);
    free(out_buf);
    (void)ver;
    (void)ret_deflate_init;
    (void)ret_deflate_reset;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)compressed_len;
    (void)file_offset;
    (void)strm.total_out;
    // API sequence test completed successfully
    return 66;
}