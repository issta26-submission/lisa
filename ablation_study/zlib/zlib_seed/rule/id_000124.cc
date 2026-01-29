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
//<ID> 124
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
    unsigned char input[128];
    unsigned char comp[256];
    unsigned char decomp[256];
    memset(input, 'A', sizeof(input));
    memset(comp, 0, sizeof(comp));
    memset(decomp, 0, sizeof(decomp));
    uLong inputLen = (uLong)sizeof(input);

    // step 2: Setup (deflate init and configure)
    deflateInit_(&def, 6, zlibVersion(), (int)sizeof(z_stream));
    def.next_in = (Bytef *)input;
    def.avail_in = (uInt)inputLen;
    def.next_out = (Bytef *)comp;
    def.avail_out = (uInt)sizeof(comp);
    deflate(&def, 0);
    uLong compLen = def.total_out;

    // step 3: Operate (copy deflate state, inflate, and write via gzdopen)
    deflateCopy(&def_copy, &def);
    def_copy.next_in = def.next_in;
    def_copy.avail_in = def.avail_in;
    def_copy.next_out = def.next_out;
    def_copy.avail_out = def.avail_out;
    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    inf.next_in = (Bytef *)comp;
    inf.avail_in = (uInt)compLen;
    inf.next_out = (Bytef *)decomp;
    inf.avail_out = (uInt)sizeof(decomp);
    inflate(&inf, 0);
    gzFile gz = gzdopen(1, "wb");
    gzwrite(gz, (voidpc)decomp, (unsigned int)inf.total_out);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&def);
    deflateEnd(&def_copy);
    inflateEnd(&inf);

    // API sequence test completed successfully
    return 66;
}