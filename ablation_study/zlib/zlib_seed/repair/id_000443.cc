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
//<ID> 443
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
    int deflate_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int inflate_back_init_ret = inflateBackInit_(&infstrm, 15, window, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef src[] = "zlib api sequence payload";
    uInt src_len = (uInt)(sizeof(src) - 1);
    uLong crc = crc32(0UL, src, src_len);
    uLong bound = deflateBound(&defstrm, (uLong)src_len);
    Bytef *dest = new Bytef[(size_t)bound];
    memset(dest, 0, (size_t)bound);

    // step 3: Operate
    defstrm.next_in = src;
    defstrm.avail_in = src_len;
    defstrm.next_out = dest;
    defstrm.avail_out = (uInt)bound;
    int deflate_ret = deflate(&defstrm, 4);
    uInt out_len = (uInt)defstrm.total_out;
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, dest, out_len);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&defstrm);
    int inflate_back_end_ret = inflateBackEnd(&infstrm);
    delete [] dest;
    (void)ver;
    (void)deflate_init_ret;
    (void)inflate_back_init_ret;
    (void)crc;
    (void)bound;
    (void)deflate_ret;
    (void)out_len;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_back_end_ret;
    // API sequence test completed successfully
    return 66;
}