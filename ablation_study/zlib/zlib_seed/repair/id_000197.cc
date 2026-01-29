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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib API sequence test payload: compress, checksum, write via gzFile, inspect error and clear it.";
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
    const char *errstr;
    int errnum;
    uLong crc;
    const z_crc_t *crc_tbl;
    z_crc_t first_table_elem;
    uLong derived_crc;

    // step 2: Setup / Initialize
    memset(&strm, 0, sizeof(strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_reset = deflateReset(&strm);

    // step 3: Operate / Validate
    bound = deflateBound(&strm, input_len);
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)((bound <= (uLong)sizeof(outbuf)) ? bound : (uLong)sizeof(outbuf));
    ret_deflate_call = deflate(&strm, 4);
    compressed_len = strm.total_out;
    crc = crc32(0L, outbuf, (uInt)compressed_len);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);
    errstr = gzerror(gz, &errnum);
    gzclearerr(gz);

    // step 4: Cleanup
    ret_gz_write = ret_gz_write; // keep variable referenced
    ret_gz_write = ret_gz_write; // no-op to satisfy potential unused-variable checks
    ret_gz_write = ret_gz_write;
    ret_gz_write = ret_gz_write;
    ret_gz_write = ret_gz_write;
    ret_gz_write = ret_gz_write;
    ret_gz_write = ret_gz_write;
    ret_gz_write = ret_gz_write;
    gzclose(gz);
    ret_deflate_end = deflateEnd(&strm);
    crc_tbl = get_crc_table();
    first_table_elem = crc_tbl ? crc_tbl[0] : (z_crc_t)0;
    derived_crc = crc + (uLong)first_table_elem;
    (void)ver;
    (void)ret_deflate_init;
    (void)ret_deflate_reset;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)errstr;
    (void)errnum;
    (void)bound;
    (void)first_table_elem;
    (void)derived_crc;
    (void)compressed_len;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}