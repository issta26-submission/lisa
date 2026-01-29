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
//<ID> 454
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    Bytef src[] = "The quick brown fox jumps over the lazy dog";
    uLong src_len = (uLong)(sizeof(src) - 1);
    Bytef compbuf[256];
    memset(compbuf, 0, sizeof(compbuf));
    uLongf comp_len = (uLongf)sizeof(compbuf);
    int compress_ret = compress(compbuf, &comp_len, src, src_len);

    // Step 2: Configure
    uLong bound = deflateBound(&strm, (uLong)comp_len);
    (void)bound;

    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb+");
    int gzwrite_ret = gzwrite(gz, compbuf, (unsigned int)comp_len);

    // Step 3: Operate
    int gzrewind_ret = gzrewind(gz);
    Bytef readbuf[512];
    memset(readbuf, 0, sizeof(readbuf));
    int gzread_ret = gzread(gz, readbuf, (unsigned int)comp_len);

    const z_crc_t *crc_table = get_crc_table();
    z_crc_t first_crc_entry = crc_table[0];

    // Step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&strm);

    (void)ver;
    (void)deflate_init_ret;
    (void)compress_ret;
    (void)comp_len;
    (void)gzwrite_ret;
    (void)gzrewind_ret;
    (void)gzread_ret;
    (void)first_crc_entry;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}