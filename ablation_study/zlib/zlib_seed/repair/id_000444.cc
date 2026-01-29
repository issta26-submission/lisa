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
//<ID> 444
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int inf_back_init_ret = inflateBackInit_(&infstrm, 15, window, ver, (int)sizeof(z_stream));

    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int deflate_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure / Prepare data
    Bytef source[] = "Example payload for deflate and crc32.";
    uLong sourceLen = (uLong)(sizeof(source) - 1);
    uLong bound = deflateBound(&defstrm, sourceLen);
    Bytef *dest = (Bytef *)malloc((size_t)bound);
    memset(dest, 0, (size_t)bound);
    defstrm.next_in = source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = dest;
    defstrm.avail_out = (uInt)bound;

    // step 3: Operate (compress, checksum, write)
    int deflate_ret = deflate(&defstrm, 4); /* 4 used for Z_FINISH */
    uLong compressed_len = defstrm.total_out;
    uLong checksum = crc32(0UL, source, (uInt)sourceLen);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, dest, (unsigned int)compressed_len);

    // step 4: Validate / Cleanup
    int inflate_back_end_ret = inflateBackEnd(&infstrm);
    int deflate_end_ret = deflateEnd(&defstrm);
    int gzclose_ret = gzclose(gz);
    free(dest);

    (void)ver;
    (void)inf_back_init_ret;
    (void)deflate_init_ret;
    (void)bound;
    (void)deflate_ret;
    (void)checksum;
    (void)filename;
    (void)gzwrite_ret;
    (void)inflate_back_end_ret;
    (void)deflate_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}