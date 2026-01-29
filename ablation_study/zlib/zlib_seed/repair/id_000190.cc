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
//<ID> 190
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib API sequence payload: compress, write to gzFile, inspect error/clear, verify CRC table.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef outbuf[262144];
    const char *ver;
    gzFile gz;
    uLong bound;
    uLong compressed_len;
    const z_crc_t *crc_table;
    uLong crc;
    int ret_deflate_init;
    int ret_deflate_reset;
    int ret_deflate_call;
    int ret_deflate_end;
    int gzwrite_ret;
    int gzclose_ret;
    int gz_errnum;
    const char *gz_errstr;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_reset = deflateReset(&strm);

    // step 3: Core operations
    bound = deflateBound(&strm, input_len);
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)((bound <= (uLong)sizeof(outbuf)) ? bound : (uLong)sizeof(outbuf));
    ret_deflate_call = deflate(&strm, 4);
    compressed_len = strm.total_out;
    crc = crc32(0L, outbuf, (uInt)compressed_len);
    crc_table = get_crc_table();
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite_ret = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);
    gz_errstr = gzerror(gz, &gz_errnum);
    gzclearerr(gz);

    // step 4: Cleanup and validation
    ret_deflate_end = deflateEnd(&strm);
    gzclose_ret = gzclose(gz);
    (void)ver;
    (void)ret_deflate_init;
    (void)ret_deflate_reset;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)gz_errnum;
    (void)gz_errstr;
    (void)crc_table;
    (void)crc;
    (void)compressed_len;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}