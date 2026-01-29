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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib: payload for deflate, write via gzFile, inspect error state and crc table.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef outbuf[65536];
    uLong bound;
    uLong compressed_len;
    const char *ver;
    gzFile gz;
    int ret_deflate_init;
    int ret_deflate_reset;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_write;
    const char *gz_errstr;
    int gz_errnum;
    uLong crc;
    const z_crc_t *crc_table;

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
    crc ^= (uLong)(crc_table ? (uLong)crc_table[0] : 0UL);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);
    gz_errstr = gzerror(gz, &gz_errnum);
    gzclearerr(gz);
    ret_gz_write = ret_gz_write; // keep variable used

    // step 4: Cleanup and validation
    ret_gz_write = ret_gz_write; // no-op to avoid unused warning
    ret_deflate_end = deflateEnd(&strm);
    gzclose(gz);
    (void)ver;
    (void)ret_deflate_init;
    (void)ret_deflate_reset;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)gz_errstr;
    (void)gz_errnum;
    (void)crc;
    (void)crc_table;
    (void)compressed_len;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}