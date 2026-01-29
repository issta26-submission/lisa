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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib: deflate + deflateBound + deflateReset + gzoffset integration test payload.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    const char * ver;
    int ret_deflate_init;
    int ret_deflate_reset;
    uLong bound;
    Bytef * outbuf = (Bytef *)0;
    int ret_deflate;
    uLong produced;
    gzFile gz = (gzFile)0;
    int ret_gz_write;
    off_t offset;
    int ret_deflate_end;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_reset = deflateReset(&strm);

    // step 3: Core operations
    bound = deflateBound(&strm, input_len);
    outbuf = (Bytef *)malloc((size_t)(bound + 16));
    strm.next_in = (Bytef *)input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)bound;
    ret_deflate = deflate(&strm, 4); /* 4 == Z_FINISH */
    produced = strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)produced);
    offset = gzoffset(gz);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    gzclose(gz);
    free(outbuf);
    (void)ret_deflate_init;
    (void)ret_deflate_reset;
    (void)ret_deflate;
    (void)ret_gz_write;
    (void)offset;
    (void)ret_deflate_end;
    (void)ver;
    (void)input_len;
    (void)strm.total_out;
    // API sequence test completed successfully
    return 66;
}