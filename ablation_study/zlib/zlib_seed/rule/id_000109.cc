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
//<ID> 109
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
    unsigned char input[128];
    unsigned char output[128];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Configure (deflate)
    deflateInit_(&def, 6, zlibVersion(), (int)sizeof(z_stream));
    def.next_in = (Bytef *)input;
    def.avail_in = (uInt)inputLen;
    def.next_out = (Bytef *)comp;
    def.avail_out = (uInt)bound;
    deflate(&def, 0);
    unsigned int comp_len = (unsigned int)def.total_out;

    // step 3: Configure & Operate (inflate)
    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    inf.next_in = (Bytef *)comp;
    inf.avail_in = (uInt)comp_len;
    inf.next_out = (Bytef *)output;
    inf.avail_out = (uInt)sizeof(output);
    inflate(&inf, 0);
    unsigned int decomp_len = (unsigned int)inf.total_out;

    // step 4: Operate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)output, decomp_len);
    gzflush(gz, 2);
    deflateEnd(&def);
    inflateEnd(&inf);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}