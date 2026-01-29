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
//<ID> 137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def;
    z_stream inf;
    memset(&def, 0, sizeof(def));
    memset(&inf, 0, sizeof(inf));
    unsigned char src[256];
    memset(src, 'A', sizeof(src));
    uLong srcLen = (uLong)sizeof(src);
    uLong bound = compressBound(srcLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    unsigned char decomp[512];
    memset(decomp, 0, sizeof(decomp));

    // step 2: Setup deflate and compress
    deflateInit_(&def, 6, zlibVersion(), (int)sizeof(z_stream));
    def.next_in = (Bytef *)src;
    def.avail_in = (uInt)srcLen;
    def.next_out = comp;
    def.avail_out = (uInt)bound;
    deflate(&def, 0);
    uLong comp_len = def.total_out;

    // step 3: Reset keep, initialize inflate, decompress and write gz
    deflateResetKeep(&def);
    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    inflateReset(&inf);
    inf.next_in = comp;
    inf.avail_in = (uInt)comp_len;
    inf.next_out = (Bytef *)decomp;
    inf.avail_out = (uInt)sizeof(decomp);
    inflate(&inf, 0);
    unsigned int decomp_len = (unsigned int)inf.total_out;
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)decomp, decomp_len);
    gzflush(gz, 2);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&def);
    inflateEnd(&inf);
    free(comp);

    return 66;
    // API sequence test completed successfully
}