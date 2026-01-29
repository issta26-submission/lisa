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
//<ID> 174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "Example payload for zlib deflate then written and inspected with gzoffset.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    unsigned char * outbuf = NULL;
    uLong outbuf_len = 0;
    gzFile gz = (gzFile)0;
    off_t file_offset = 0;
    const char * ver = (const char *)0;
    int ret_deflate_init = 0;
    int ret_def_reset = 0;
    int ret_deflate = 0;
    int ret_deflate_end = 0;
    int ret_gz_write = 0;
    int ret_gz_close = 0;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ver, (int)sizeof(z_stream));
    ret_def_reset = deflateReset(&strm);
    outbuf_len = deflateBound(&strm, input_len);
    outbuf = (unsigned char *)malloc((size_t)outbuf_len);
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)outbuf_len;

    // step 3: Core operations
    ret_deflate = deflate(&strm, Z_FINISH);
    uLong compressed_size = strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_size);
    ret_gz_close = gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    file_offset = gzoffset(gz);
    ret_gz_close = gzclose(gz);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    free(outbuf);
    (void)ret_deflate_init;
    (void)ret_def_reset;
    (void)ret_deflate;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)ver;
    (void)input_len;
    (void)compressed_size;
    (void)file_offset;
    // API sequence test completed successfully
    return 66;
}