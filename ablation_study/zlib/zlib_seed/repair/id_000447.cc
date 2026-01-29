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
//<ID> 447
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int deflate_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int inflate_back_init_ret = inflateBackInit_(&infstrm, 15, window, ver, (int)sizeof(z_stream));

    // step 2: Configure
    const Bytef input_data[] = "Example input data for deflate and crc32.";
    uLong input_len = (uLong)(sizeof(input_data) - 1);
    uLong input_crc = crc32(0L, input_data, (uInt)input_len);
    defstrm.next_in = (Bytef *)input_data;
    defstrm.avail_in = (uInt)input_len;
    uLong bound = deflateBound(&defstrm, input_len);
    Bytef outbuf[16384];
    memset(outbuf, 0, sizeof(outbuf));
    defstrm.next_out = outbuf;
    defstrm.avail_out = (uInt)sizeof(outbuf);

    // step 3: Operate
    int deflate_ret = deflate(&defstrm, 4); /* 4 = Z_FINISH */
    unsigned int compressed_size = (unsigned int)defstrm.total_out;
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret1 = gzwrite(gz, outbuf, compressed_size);
    int gzwrite_ret2 = gzwrite(gz, &input_crc, (unsigned int)sizeof(input_crc));

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&defstrm);
    int inflate_back_end_ret = inflateBackEnd(&infstrm);
    int gzclose_ret = gzclose(gz);
    (void)ver;
    (void)deflate_init_ret;
    (void)inflate_back_init_ret;
    (void)bound;
    (void)deflate_ret;
    (void)gzwrite_ret1;
    (void)gzwrite_ret2;
    (void)deflate_end_ret;
    (void)inflate_back_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}