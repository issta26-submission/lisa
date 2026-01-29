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
//<ID> 440
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
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int inf_init_ret = inflateBackInit_(&infstrm, 15, window, ver, (int)sizeof(z_stream));

    // step 2: Configure
    const Bytef source[] = "Example payload data for zlib test.";
    uInt source_len = (uInt)(sizeof(source) - 1);
    uLong bound = deflateBound(&defstrm, (uLong)source_len);
    Bytef *outbuf = new Bytef[(size_t)bound];
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = source_len;
    defstrm.next_out = outbuf;
    defstrm.avail_out = (uInt)bound;

    // step 3: Operate
    uLong checksum = crc32(0UL, source, source_len);
    int def_ret = deflate(&defstrm, 0);
    uLong compressed_size = defstrm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    unsigned int write_len = (unsigned int)compressed_size;
    int gzwrite_ret = gzwrite(gz, outbuf, write_len);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int def_end_ret = deflateEnd(&defstrm);
    int inf_back_end_ret = inflateBackEnd(&infstrm);
    delete[] outbuf;
    (void)ver;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)checksum;
    (void)def_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)def_end_ret;
    (void)inf_back_end_ret;
    // API sequence test completed successfully
    return 66;
}