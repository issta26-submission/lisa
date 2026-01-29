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
//<ID> 448
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
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    unsigned char window[1 << 15];
    int inflate_back_init_ret = inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    int deflate_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef src[] = "Example payload for zlib deflate and crc32.";
    uInt src_len = (uInt)(sizeof(src) - 1);
    uLong checksum = crc32(0UL, src, src_len);
    uLong bound = deflateBound(&defstrm, (uLong)src_len);
    Bytef dest[1024];
    defstrm.next_in = src;
    defstrm.avail_in = src_len;
    defstrm.next_out = dest;
    defstrm.avail_out = (uInt)sizeof(dest);

    // step 3: Operate
    int deflate_ret = deflate(&defstrm, 4); // 4 == Z_FINISH
    uLong out_len = defstrm.total_out;
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, dest, (unsigned int)out_len);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&defstrm);
    int inflate_back_end_ret = inflateBackEnd(&backstrm);
    (void)ver;
    (void)inflate_back_init_ret;
    (void)deflate_init_ret;
    (void)checksum;
    (void)bound;
    (void)deflate_ret;
    (void)out_len;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_back_end_ret;
    (void)filename;
    // API sequence test completed successfully
    return 66;
}