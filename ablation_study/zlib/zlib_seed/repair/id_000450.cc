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
//<ID> 450
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
    Bytef input[256];
    memset(input, 'A', sizeof(input));
    strm.next_in = input;
    strm.avail_in = (uInt)sizeof(input);
    uLong bound = deflateBound(&strm, (uLong)strm.avail_in);
    Bytef compbuf[16384];
    memset(compbuf, 0, sizeof(compbuf));
    uLongf compLen = (uLongf)sizeof(compbuf);
    int compress_ret = compress(compbuf, &compLen, input, (uLong)strm.avail_in);
    const z_crc_t *crc_table = get_crc_table();
    uLong crc_seed = (uLong)crc_table[0];
    compbuf[0] = (Bytef)(compbuf[0] ^ (crc_seed & 0xFF));

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, compbuf, (unsigned int)compLen);
    int gzclose_w_ret = gzclose(gz);
    gzFile gz_r = gzopen(filename, "rb");
    Bytef readbuf[16384];
    memset(readbuf, 0, sizeof(readbuf));
    int gzread_ret = gzread(gz_r, readbuf, (unsigned int)sizeof(readbuf));
    int gzclose_r_ret = gzclose(gz_r);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)bound;
    (void)compress_ret;
    (void)crc_table;
    (void)crc_seed;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzread_ret;
    (void)gzclose_r_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}