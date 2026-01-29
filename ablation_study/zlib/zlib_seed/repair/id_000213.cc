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
//<ID> 213
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
    Bytef input_buf[] = "zlib API sequence test payload: compress, prime, write via gzFile, compute CRC, init inflateBack, then cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef outbuf[65536];
    unsigned char window_buf[32768];
    const char *ver;
    gzFile gz;
    uLong bound;
    uLong compressed_len;
    uLong checksum;
    int ret_deflate_init;
    int ret_deflate_prime;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_inflate_back_init;
    int ret_inflate_end;
    int ret_gz_write;
    int ret_gz_close;

    // step 2: Setup
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(outbuf, 0, sizeof(outbuf));
    memset(window_buf, 0, sizeof(window_buf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_prime = deflatePrime(&def_strm, 1, 1);

    // step 3: Core operations
    bound = deflateBound(&def_strm, input_len);
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = outbuf;
    def_strm.avail_out = (uInt)((bound <= (uLong)sizeof(outbuf)) ? bound : (uLong)sizeof(outbuf));
    ret_deflate_call = deflate(&def_strm, 4);
    compressed_len = def_strm.total_out;
    checksum = crc32(0UL, outbuf, (uInt)compressed_len);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);
    ret_gz_close = gzclose(gz);

    // step 4: InflateBack init and cleanup
    ret_inflate_back_init = inflateBackInit_(&inf_strm, 15, window_buf, ver, (int)sizeof(z_stream));
    ret_inflate_end = inflateEnd(&inf_strm);
    ret_deflate_end = deflateEnd(&def_strm);

    // API sequence test completed successfully
    (void)ret_deflate_init;
    (void)ret_deflate_prime;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_inflate_back_init;
    (void)ret_inflate_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)checksum;
    (void)bound;
    (void)compressed_len;
    (void)input_len;
    (void)ver;
    return 66;
}