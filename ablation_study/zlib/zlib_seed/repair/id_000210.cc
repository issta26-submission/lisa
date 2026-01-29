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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream def_strm;
    z_stream back_strm;
    Bytef input_buf[] = "zlib API sequence test payload for compress->write and inflateBack init/cleanup with CRC check.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[65536];
    unsigned char window[32768];
    const char *ver;
    gzFile gz;
    uLong bound;
    uLong compressed_len;
    int ret_deflate_init;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_inflateBackInit;
    int ret_inflateEnd;
    int ret_gzwrite;
    uLong checksum;

    // step 2: Setup
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&back_strm, 0, sizeof(back_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(window, 0, sizeof(window));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 3: Core operations
    bound = deflateBound(&def_strm, input_len);
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)((bound <= (uLong)sizeof(comp_buf)) ? bound : (uLong)sizeof(comp_buf));
    deflatePrime(&def_strm, 3, 0);
    ret_deflate_call = deflate(&def_strm, 4);
    compressed_len = def_strm.total_out;
    checksum = crc32(0UL, input_buf, (uInt)input_len);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gzwrite = gzwrite(gz, (voidpc)comp_buf, (unsigned int)compressed_len);
    ret_inflateBackInit = inflateBackInit_(&back_strm, 15, window, ver, (int)sizeof(z_stream));

    // step 4: Cleanup and validation
    ret_inflateEnd = inflateEnd(&back_strm);
    ret_deflate_end = deflateEnd(&def_strm);
    gzclose(gz);
    (void)ret_deflate_init;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_inflateBackInit;
    (void)ret_inflateEnd;
    (void)ret_gzwrite;
    (void)bound;
    (void)compressed_len;
    (void)checksum;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}