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
//<ID> 459
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef src[128];
    memset(src, 'A', sizeof(src));
    Bytef dest[65536];
    memset(dest, 0, sizeof(dest));
    uLongf destLen = (uLongf)sizeof(dest);
    int compress_ret = compress(dest, &destLen, src, (uLong)sizeof(src));
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    strm.next_in = src;
    strm.avail_in = (uInt)sizeof(src);
    strm.next_out = dest;
    strm.avail_out = (uInt)destLen;
    const z_crc_t *crc_tbl = get_crc_table();
    (void)crc_tbl;

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gzw = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzw, dest, (unsigned int)destLen);
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen(filename, "rb");
    Bytef readbuf[65536];
    memset(readbuf, 0, sizeof(readbuf));
    int gzread_ret = gzread(gzr, (voidp)readbuf, (unsigned int)destLen);
    int gzclose_r_ret = gzclose(gzr);

    // step 4: Validate / Cleanup
    uLong crc_seed = (uLong)crc_tbl[0];
    uLong checksum = crc32(crc_seed, readbuf, (uInt)gzread_ret);
    int deflate_end_ret = deflateEnd(&strm);

    (void)ver;
    (void)compress_ret;
    (void)deflate_init_ret;
    (void)strm;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzread_ret;
    (void)gzclose_r_ret;
    (void)checksum;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}