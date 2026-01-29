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
//<ID> 453
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
    Bytef src[256];
    memset(src, 'A', sizeof(src));
    uLong srcLen = (uLong)sizeof(src);
    uLongf destLen = (uLongf)compressBound(srcLen);
    Bytef dest[1024];
    memset(dest, 0, sizeof(dest));
    int compress_ret = compress(dest, &destLen, src, srcLen);

    // step 3: Operate
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb+");
    int gzwrite_ret = gzwrite(gz, src, (unsigned int)srcLen);
    int gzrewind_ret = gzrewind(gz);
    Bytef readbuf[256];
    memset(readbuf, 0, sizeof(readbuf));
    int gzread_ret = gzread(gz, (voidp)readbuf, (unsigned int)sizeof(readbuf));
    const z_crc_t *crc_tab = get_crc_table();
    uLong crc_src = crc32(0L, src, (uInt)srcLen);
    uLong crc_read = crc32(0L, readbuf, (uInt)sizeof(readbuf));
    uLong combined = crc_src ^ crc_read ^ (uLong)crc_tab[0];

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&strm);

    (void)ver;
    (void)deflate_init_ret;
    (void)destLen;
    (void)compress_ret;
    (void)gzwrite_ret;
    (void)gzrewind_ret;
    (void)gzread_ret;
    (void)crc_tab;
    (void)crc_src;
    (void)crc_read;
    (void)combined;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}