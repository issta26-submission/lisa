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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    const char *ver;
    Bytef gzip_data[] = {
        0x1f,0x8b,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
        0xf3,0x48,0xcd,0xc9,0xc9,0xd7,0x51,0x08,0xcf,0x2f,
        0xca,0x49,0x01,0x00,0x85,0x11,0x4a,0x0b,0x06,0x00,0x00,0x00
    };
    uLong gzip_len = (uLong)sizeof(gzip_data);
    Bytef outbuf[64];
    uLong decompressed_len;
    uLong checksum;
    gzFile gz;
    int ret_inflate_init2;
    int ret_inflate_call;
    int ret_gz_write;
    int ret_gz_close;
    int ret_inflate_end;

    // step 2: Setup
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_inflate_init2 = inflateInit2_(&inf_strm, 15 + 16, ver, (int)sizeof(z_stream));
    inf_strm.next_in = gzip_data;
    inf_strm.avail_in = (uInt)gzip_len;
    inf_strm.next_out = outbuf;
    inf_strm.avail_out = (uInt)sizeof(outbuf);

    // step 3: Core operations
    ret_inflate_call = inflate(&inf_strm, 4);
    decompressed_len = inf_strm.total_out;
    checksum = crc32(0L, outbuf, (uInt)decompressed_len);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)decompressed_len);

    // step 4: Cleanup and validation
    ret_gz_close = gzclose(gz);
    ret_inflate_end = inflateEnd(&inf_strm);
    (void)ret_inflate_init2;
    (void)ret_inflate_call;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)ret_inflate_end;
    (void)checksum;
    (void)decompressed_len;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}