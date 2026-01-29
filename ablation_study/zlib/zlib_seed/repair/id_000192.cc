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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib API sequence test payload: compress, write via gzFile, inspect error state and CRC table.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef outbuf[131072];
    uLong bound;
    uLong compressed_len;
    const char * ver;
    gzFile gz;
    int ret_deflate_init;
    int ret_deflate_reset;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_write;
    int gz_errnum;
    const char * gz_errstr;
    const z_crc_t * crc_tab;
    uLong crc;
    (void)gz_errnum;

    // step 2: Setup (Initialize)
    memset(&strm, 0, sizeof(strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_reset = deflateReset(&strm);

    // step 3: Core operations (Configure -> Operate)
    bound = deflateBound(&strm, input_len);
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)((bound <= (uLong)sizeof(outbuf)) ? bound : (uLong)sizeof(outbuf));
    ret_deflate_call = deflate(&strm, 4);
    compressed_len = strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);
    gz_errstr = gzerror(gz, &gz_errnum);
    gzclearerr(gz);
    crc_tab = get_crc_table();
    crc = crc32(0L, input_buf, (uInt)input_len);
    crc ^= (uLong)(crc_tab ? crc_tab[0] : 0u);

    // step 4: Cleanup and validation
    ret_deflate_end = deflateEnd(&strm);
    gzclose(gz);
    (void)ret_deflate_init;
    (void)ret_deflate_reset;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)gz_errstr;
    (void)crc;
    (void)ver;
    (void)compressed_len;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}