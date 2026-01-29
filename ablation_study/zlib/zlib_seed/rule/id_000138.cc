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
//<ID> 138
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_stream;
    z_stream inf_stream;
    memset(&def_stream, 0, sizeof(def_stream));
    memset(&inf_stream, 0, sizeof(inf_stream));
    unsigned char input[256];
    unsigned char output[512];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Setup (deflate init and compress)
    deflateInit_(&def_stream, 6, zlibVersion(), (int)sizeof(z_stream));
    def_stream.next_in = (Bytef *)input;
    def_stream.avail_in = (uInt)inputLen;
    def_stream.next_out = comp;
    def_stream.avail_out = (uInt)bound;
    deflate(&def_stream, Z_FINISH);
    uLongf compLen = (uLongf)def_stream.total_out;
    deflateResetKeep(&def_stream);

    // step 3: Configure (inflate init and reset) and Decompress
    inflateInit_(&inf_stream, zlibVersion(), (int)sizeof(z_stream));
    inflateReset(&inf_stream);
    inf_stream.next_in = comp;
    inf_stream.avail_in = (uInt)compLen;
    inf_stream.next_out = (Bytef *)output;
    inf_stream.avail_out = (uInt)sizeof(output);
    inflate(&inf_stream, Z_FINISH);
    uLong decompLen = inf_stream.total_out;

    // step 4: Operate (write decompressed data to gz using gzopen64, flush) & Cleanup
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)output, (unsigned int)decompLen);
    gzflush(gz, Z_SYNC_FLUSH);
    gzclose(gz);
    deflateEnd(&def_stream);
    inflateEnd(&inf_stream);
    free(comp);

    // API sequence test completed successfully
    return 66;
}