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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def;
    z_stream def_copy;
    z_stream inf;
    memset(&def, 0, sizeof(def));
    memset(&def_copy, 0, sizeof(def_copy));
    memset(&inf, 0, sizeof(inf));
    unsigned char input[256];
    unsigned char output[512];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Setup (deflate init and compress)
    deflateInit_(&def, 6, zlibVersion(), (int)sizeof(z_stream));
    def.next_in = (Bytef *)input;
    def.avail_in = (uInt)inputLen;
    def.next_out = comp;
    def.avail_out = (uInt)bound;
    deflate(&def, 0);

    // step 3: Copy state and initialize inflate, then decompress
    deflateCopy(&def_copy, &def);
    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    inf.next_in = comp;
    inf.avail_in = (uInt)def.total_out;
    inf.next_out = output;
    inf.avail_out = (uInt)sizeof(output);
    inflate(&inf, 0);

    // step 4: Write out via gz, cleanup
    gzFile gz = gzdopen(1, "wb");
    gzwrite(gz, (voidpc)output, (unsigned int)inf.total_out);
    gzclose(gz);
    deflateEnd(&def);
    deflateEnd(&def_copy);
    inflateEnd(&inf);
    free(comp);

    // API sequence test completed successfully
    return 66;
}