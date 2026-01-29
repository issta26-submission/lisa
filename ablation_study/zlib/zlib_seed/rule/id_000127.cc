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
//<ID> 127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src;
    z_stream dst;
    z_stream inf;
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    memset(&inf, 0, sizeof(inf));
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Configure (deflate source)
    deflateInit_(&src, 6, zlibVersion(), (int)sizeof(z_stream));
    src.next_in = (Bytef *)input;
    src.avail_in = (uInt)inputLen;
    src.next_out = comp;
    src.avail_out = (uInt)bound;
    deflate(&src, 0);
    uLong first_out = src.total_out;

    // step 3: Copy state and continue deflation, initialize inflater, write out via gz
    deflateCopy(&dst, &src);
    dst.next_out = comp + (uInt)first_out;
    dst.avail_out = (uInt)(bound - first_out);
    deflate(&dst, 0);
    uLong total_out = dst.total_out;
    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    gzFile gz = gzdopen(1, "wb");
    gzwrite(gz, (voidpc)comp, (unsigned int)total_out);

    // step 4: Cleanup
    deflateEnd(&src);
    deflateEnd(&dst);
    inflateEnd(&inf);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}