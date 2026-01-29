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
//<ID> 217
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
    z_stream back_strm;
    Bytef input_buf[] = "zlib API sequence test payload: compress, prime, write via gzFile, compute CRC, init inflateBack, cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef outbuf[65536];
    unsigned char window[32768];
    const char *ver;
    gzFile gz;
    uLong bound;
    uLong compressed_len;
    uLong checksum;
    int ret_def_init;
    int ret_prime;
    int ret_deflate;
    int ret_def_end;
    int ret_gz_write;
    int ret_gz_close;
    int ret_inf_init;
    int ret_inf_end;
    int ret_back_init;
    int ret_back_end;

    // step 2: Setup
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&back_strm, 0, sizeof(back_strm));
    memset(outbuf, 0, sizeof(outbuf));
    memset(window, 0, sizeof(window));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_prime = deflatePrime(&def_strm, 3, 0x5);

    // step 3: Core operations
    bound = deflateBound(&def_strm, input_len);
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = outbuf;
    def_strm.avail_out = (uInt)((bound <= (uLong)sizeof(outbuf)) ? bound : (uLong)sizeof(outbuf));
    ret_deflate = deflate(&def_strm, 4);
    compressed_len = def_strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);
    ret_gz_close = gzclose(gz);
    checksum = crc32(0UL, outbuf, (uInt)compressed_len);
    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    ret_inf_end = inflateEnd(&inf_strm);
    ret_back_init = inflateBackInit_(&back_strm, 15, window, ver, (int)sizeof(z_stream));
    ret_back_end = inflateBackEnd(&back_strm);

    // step 4: Cleanup and validation
    ret_def_end = deflateEnd(&def_strm);
    (void)ret_def_init;
    (void)ret_prime;
    (void)ret_deflate;
    (void)ret_def_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)ret_inf_init;
    (void)ret_inf_end;
    (void)ret_back_init;
    (void)ret_back_end;
    (void)ver;
    (void)bound;
    (void)checksum;
    (void)compressed_len;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}