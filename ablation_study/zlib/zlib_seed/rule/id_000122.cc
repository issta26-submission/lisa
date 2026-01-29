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
//<ID> 122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src;
    z_stream copy;
    z_stream inf;
    memset(&src, 0, sizeof(src));
    memset(&copy, 0, sizeof(copy));
    memset(&inf, 0, sizeof(inf));
    unsigned char input[128];
    unsigned char comp[512];
    unsigned char output[256];
    memset(input, 'A', sizeof(input));
    memset(comp, 0, sizeof(comp));
    memset(output, 0, sizeof(output));
    uLong in_len = (uLong)sizeof(input);

    // step 2: Configure (deflate init and first deflate)
    deflateInit_(&src, 6, zlibVersion(), (int)sizeof(z_stream));
    src.next_in = (Bytef *)input;
    src.avail_in = (uInt)in_len;
    src.next_out = (Bytef *)comp;
    src.avail_out = (uInt)sizeof(comp);
    deflate(&src, 0);
    unsigned int first_comp_len = (unsigned int)src.total_out;

    // step 3: Copy deflate state and continue compression, then initialize inflate and decompress
    deflateCopy(&copy, &src);
    copy.next_out = (Bytef *)(comp + first_comp_len);
    copy.avail_out = (uInt)(sizeof(comp) - first_comp_len);
    copy.next_in = src.next_in;
    copy.avail_in = src.avail_in;
    deflate(&copy, 0);
    unsigned int total_comp_len = first_comp_len + (unsigned int)copy.total_out;

    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    inf.next_in = (Bytef *)comp;
    inf.avail_in = (uInt)total_comp_len;
    inf.next_out = (Bytef *)output;
    inf.avail_out = (uInt)sizeof(output);
    inflate(&inf, 0);
    unsigned int decomp_len = (unsigned int)inf.total_out;

    // step 4: Operate (write through gz descriptor) & Cleanup
    gzFile gz = gzdopen(1, "wb");
    gzwrite(gz, (voidpc)output, decomp_len);
    gzclose(gz);
    deflateEnd(&src);
    deflateEnd(&copy);
    inflateEnd(&inf);

    // API sequence test completed successfully
    return 66;
}