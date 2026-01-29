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
//<ID> 458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    const Bytef source[] = "Example payload for zlib compress and gz read test.";
    uLong sourceLen = (uLong)(sizeof(source) - 1);
    Bytef compressed[512];
    uLongf compressedLen = (uLongf)sizeof(compressed);
    int compress_ret = compress(compressed, &compressedLen, source, sourceLen);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t crc0 = crc_tbl[0];

    // step 3: Operate
    const char *fname = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(fname, "wb");
    int gzwrite_ret = gzwrite(gz, compressed, (unsigned int)compressedLen);
    int gzclose_ret = gzclose(gz);
    gzFile gz2 = gzopen(fname, "rb");
    Bytef readbuf[512];
    memset(readbuf, 0, sizeof(readbuf));
    int gzread_ret = gzread(gz2, (voidp)readbuf, (unsigned int)compressedLen);
    int gzclose2_ret = gzclose(gz2);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)compress_ret;
    (void)crc0;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)gzread_ret;
    (void)gzclose2_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}