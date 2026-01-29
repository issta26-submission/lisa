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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib: a sample payload for deflate, crc32 calculation and gzseek tests.";
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
    off_t off_short;
    off64_t off_long;
    uLong crc_orig;
    uLong crc_comp;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_params = deflateParams(&strm, 6, 0);

    // step 3: Core operations
    bound = deflateBound(&strm, input_len);
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)((bound <= (uLong)sizeof(outbuf)) ? bound : (uLong)sizeof(outbuf));
    ret_deflate_call = deflate(&strm, 4);
    uLong compressed_len = strm.total_out;
    crc_orig = crc32(0L, (const Bytef *)input_buf, (uInt)input_len);
    crc_comp = crc32(0L, (const Bytef *)outbuf, (uInt)compressed_len);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);
    off_short = gzseek(gz, (off_t)0, SEEK_SET);
    off_long = gzseek64(gz, (off64_t)0, SEEK_SET);
    ret_gz_close = gzclose(gz);

    // step 4: Cleanup and validation
    ret_deflate_end = deflateEnd(&strm);
    (void)ret_deflate_init;
    (void)ret_deflate_params;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)off_short;
    (void)off_long;
    (void)crc_orig;
    (void)crc_comp;
    (void)ver;
    (void)input_len;
    (void)compressed_len;
    // API sequence test completed successfully
    return 66;
}