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
//<ID> 110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def;
    z_stream back;
    memset(&def, 0, sizeof(def));
    memset(&back, 0, sizeof(back));
    unsigned char input[128];
    unsigned char output[128];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Configure
    deflateInit2_(&def, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate & Validate
    compress((Bytef *)comp, &compLen, (const Bytef *)input, inputLen);
    inflateBack(&back, (in_func)NULL, NULL, (out_func)NULL, NULL);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, "zlib test payload");
    gzputs(gz, (const char *)input);

    // step 4: Cleanup
    deflateEnd(&def);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}