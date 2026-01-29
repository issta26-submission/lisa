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
//<ID> 201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream infstrm;
    Bytef input_buf[] = "zlib API sequence test payload: compress, write with gzopen64, rewind, checksum, cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef outbuf[65536];
    uLong bound;
    uLong compressed_len;
    const char *ver;
    gzFile gz;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_write;
    int ret_gz_rewind;
    uLong checksum;

    // step 2: Setup / Initialize
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    ret_inflate_init = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));

    // step 3: Operate / Configure
    bound = deflateBound(&defstrm, input_len);
    defstrm.next_in = input_buf;
    defstrm.avail_in = (uInt)input_len;
    defstrm.next_out = outbuf;
    defstrm.avail_out = (uInt)((bound <= (uLong)sizeof(outbuf)) ? bound : (uLong)sizeof(outbuf));
    ret_deflate_call = deflate(&defstrm, 4);
    compressed_len = defstrm.total_out;
    gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);
    ret_gz_rewind = gzrewind(gz);
    checksum = crc32(0L, outbuf, (uInt)compressed_len);

    // step 4: Cleanup and validation
    ret_deflate_end = deflateEnd(&defstrm);
    inflateBackEnd(&infstrm);
    gzclose(gz);
    (void)ret_deflate_init;
    (void)ret_inflate_init;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_rewind;
    (void)checksum;
    (void)ver;
    (void)compressed_len;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}