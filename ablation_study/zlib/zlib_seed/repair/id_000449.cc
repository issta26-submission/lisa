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
//<ID> 449
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream in_strm;
    memset(&in_strm, 0, sizeof(in_strm));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int inflate_back_init_ret = inflateBackInit_(&in_strm, 15, window, ver, (int)sizeof(z_stream));

    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int deflate_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef input[1024];
    memset(input, 'A', sizeof(input));
    def_strm.next_in = input;
    def_strm.avail_in = (uInt)sizeof(input);

    uLong bound = deflateBound(&def_strm, (uLong)def_strm.avail_in);
    (void)bound; // bound is computed and used conceptually for sizing

    Bytef outbuf[65536];
    memset(outbuf, 0, sizeof(outbuf));
    def_strm.next_out = outbuf;
    def_strm.avail_out = (uInt)sizeof(outbuf);

    // step 3: Operate
    int deflate_ret = deflate(&def_strm, 4); // 4 == Z_FINISH
    uLong produced = def_strm.total_out;
    uLong checksum = crc32(0L, outbuf, (uInt)produced);

    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, outbuf, (unsigned int)produced);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&def_strm);
    int inflate_back_end_ret = inflateBackEnd(&in_strm);

    (void)inflate_back_init_ret;
    (void)deflate_init_ret;
    (void)deflate_ret;
    (void)produced;
    (void)checksum;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_back_end_ret;
    // API sequence test completed successfully
    return 66;
}