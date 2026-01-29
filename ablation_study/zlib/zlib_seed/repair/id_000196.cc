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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib API sequence test payload: compress, write via gzFile, inspect error and CRC table.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef outbuf[65536];
    uLong bound;
    uLong compressed_len;
    const char *ver;
    const z_crc_t *crc_tbl;
    z_crc_t crc_first;
    gzFile gz;
    int ret_deflate_init;
    int ret_deflate_reset;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_write;
    int ret_gz_flush;
    const char *gz_errstr;
    int gz_errnum;
    int ret_gz_close;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_reset = deflateReset(&strm);
    crc_tbl = get_crc_table();
    crc_first = crc_tbl[0];

    // step 3: Core operations
    bound = deflateBound(&strm, input_len);
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)((bound <= (uLong)sizeof(outbuf)) ? bound : (uLong)sizeof(outbuf));
    ret_deflate_call = deflate(&strm, 4);
    compressed_len = strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);
    ret_gz_flush = gzflush(gz, 2);
    gz_errstr = gzerror(gz, &gz_errnum);
    gzclearerr(gz);
    ret_gz_close = gzclose(gz);

    // step 4: Cleanup and validation
    ret_deflate_end = deflateEnd(&strm);
    (void)ret_deflate_init;
    (void)ret_deflate_reset;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_flush;
    (void)gz_errstr;
    (void)gz_errnum;
    (void)ret_gz_close;
    (void)crc_first;
    (void)ver;
    (void)compressed_len;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}