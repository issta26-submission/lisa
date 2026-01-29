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
//<ID> 130
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
    unsigned char input[128];
    unsigned char output[256];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup (compress and initialize streams)
    compress2(comp, &compLen, (const Bytef *)input, inputLen, 6);
    deflateInit_(&def_stream, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&def_stream);
    inflateInit_(&inf_stream, zlibVersion(), (int)sizeof(z_stream));
    inf_stream.next_in = comp;
    inf_stream.avail_in = (uInt)compLen;
    inf_stream.next_out = (Bytef *)output;
    inf_stream.avail_out = (uInt)sizeof(output);

    // step 3: Core operations (reset inflate, inflate, write gz and flush)
    inflateReset(&inf_stream);
    inflate(&inf_stream, 0);
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)output, (unsigned int)inf_stream.total_out);
    gzflush(gz, 2);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&def_stream);
    inflateEnd(&inf_stream);
    free(comp);

    // API sequence test completed successfully
    return 66;
}