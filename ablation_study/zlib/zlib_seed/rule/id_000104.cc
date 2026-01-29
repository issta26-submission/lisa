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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Configure (compress source into comp)
    compress(comp, &compLen, (const Bytef *)input, inputLen);

    // step 3: Operate & Validate (inflate compressed data and write out via gz)
    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    inf.next_in = comp;
    inf.avail_in = (uInt)compLen;
    unsigned char out[128];
    memset(out, 0, sizeof(out));
    inf.next_out = out;
    inf.avail_out = (uInt)sizeof(out);
    inflate(&inf, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)out, (unsigned int)inf.total_out);
    gzflush(gz, 2);

    // step 4: Cleanup
    inflateEnd(&inf);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}