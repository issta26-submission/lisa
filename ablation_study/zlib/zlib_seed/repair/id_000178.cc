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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib deflate then write compressed output to gzFile and query offset.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    const char * ver;
    int ret_deflate_init;
    int ret_deflate_reset;
    int ret_deflate_call;
    int ret_deflate_end;
    uLong bound;
    Bytef * dest;
    gzFile gz;
    int ret_gz_write;
    int ret_gz_close;
    off_t offset;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_reset = deflateReset(&strm);
    bound = deflateBound(&strm, input_len);
    dest = (Bytef *)malloc((size_t)bound);
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = dest;
    strm.avail_out = (uInt)bound;

    // step 3: Core operations
    ret_deflate_call = deflate(&strm, 4);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, dest, (unsigned int)strm.total_out);
    offset = gzoffset(gz);
    ret_gz_close = gzclose(gz);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    free(dest);
    (void)ret_deflate_init;
    (void)ret_deflate_reset;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)offset;
    (void)ver;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}