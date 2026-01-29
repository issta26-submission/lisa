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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream def_strm;
    z_stream inf_strm;
    Bytef input_buf[] = "zlib API sequence test payload: compress, write via gzFile, compute CRC, init inflateBack and cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[65536];
    unsigned char window[32768];
    const char *ver;
    gzFile gz;
    uLong comp_bound;
    uLong compressed_len;
    uLong checksum;
    int ret_deflate_init;
    int ret_deflate_prime;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_write;
    int ret_gz_close;
    int ret_inflate_back_init;
    int ret_inflate_end;

    // step 2: Setup
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(window, 0, sizeof(window));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    comp_bound = deflateBound(&def_strm, input_len);
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)((comp_bound <= (uLong)sizeof(comp_buf)) ? comp_bound : (uLong)sizeof(comp_buf));
    ret_deflate_prime = deflatePrime(&def_strm, 1, 0);

    // step 3: Core operations
    ret_deflate_call = deflate(&def_strm, 4);
    compressed_len = def_strm.total_out;
    checksum = crc32(0UL, input_buf, (uInt)input_len);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)comp_buf, (unsigned int)compressed_len);
    ret_gz_close = gzclose(gz);
    ret_inflate_back_init = inflateBackInit_(&inf_strm, 15, window, ver, (int)sizeof(z_stream));
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Cleanup and validation
    ret_deflate_end = deflateEnd(&def_strm);
    (void)ret_deflate_init;
    (void)ret_deflate_prime;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)ret_inflate_back_init;
    (void)ret_inflate_end;
    (void)compressed_len;
    (void)comp_bound;
    (void)checksum;
    (void)input_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}