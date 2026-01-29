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
//<ID> 216
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
    z_stream inf_back_strm;
    Bytef input_buf[] = "zlib API sequence test payload to compress, compute CRC, prime deflate bits, and write compressed output via gzFile.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef outbuf[65536];
    const char *ver;
    gzFile gz;
    uLong checksum;
    uLong compressed_len;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_inflateback_init;
    int ret_deflate_prime;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_inflate_end;
    int ret_inflateback_end;
    int ret_gz_write;
    int ret_gz_close;

    // step 2: Setup
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&inf_back_strm, 0, sizeof(inf_back_strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    ret_inflateback_init = inflateBackInit_(&inf_back_strm, 15, (unsigned char *)NULL, ver, (int)sizeof(z_stream));

    // step 3: Core operations
    checksum = crc32(0L, input_buf, (uInt)input_len);
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = outbuf;
    def_strm.avail_out = (uInt)sizeof(outbuf);
    ret_deflate_prime = deflatePrime(&def_strm, 3, 5);
    ret_deflate_call = deflate(&def_strm, 4);
    compressed_len = def_strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);

    // step 4: Cleanup and validation
    ret_deflate_end = deflateEnd(&def_strm);
    ret_inflate_end = inflateEnd(&inf_strm);
    ret_inflateback_end = inflateBackEnd(&inf_back_strm);
    ret_gz_close = gzclose(gz);
    (void)ret_deflate_init;
    (void)ret_inflate_init;
    (void)ret_inflateback_init;
    (void)checksum;
    (void)ret_deflate_prime;
    (void)ret_deflate_call;
    (void)compressed_len;
    (void)ret_deflate_end;
    (void)ret_inflate_end;
    (void)ret_inflateback_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}