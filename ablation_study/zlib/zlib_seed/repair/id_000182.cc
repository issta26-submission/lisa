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
//<ID> 182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib integration test: compress with deflate, adjust params, write via gz, seek, compute CRCs.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef outbuf[65536];
    uLong bound;
    const char * ver;
    gzFile gz;
    int ret_deflate_init;
    int ret_deflate_params;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_write;
    int ret_gz_close;
    off_t seek_res;
    off64_t seek64_res;
    uLong compressed_len;
    uLong crc_in;
    uLong crc_out;

    // step 2: Setup / Initialize
    memset(&strm, 0, sizeof(strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_params = deflateParams(&strm, 9, 0);

    // step 3: Operate (compress, write, seek, CRC)
    bound = deflateBound(&strm, input_len);
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);
    ret_deflate_call = deflate(&strm, 4);
    compressed_len = strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);
    seek_res = gzseek(gz, (off_t)0, 0);
    seek64_res = gzseek64(gz, (off64_t)0, 0);
    crc_in = crc32(0L, input_buf, (uInt)input_len);
    crc_out = crc32(0L, outbuf, (uInt)compressed_len);
    ret_gz_close = gzclose(gz);

    // step 4: Cleanup / Validate
    ret_deflate_end = deflateEnd(&strm);
    (void)ret_deflate_init;
    (void)ret_deflate_params;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)seek_res;
    (void)seek64_res;
    (void)bound;
    (void)ver;
    (void)compressed_len;
    (void)crc_in;
    (void)crc_out;
    // API sequence test completed successfully
    return 66;
}