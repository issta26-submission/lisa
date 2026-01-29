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
//<ID> 134
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream instrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&instrm, 0, sizeof(instrm));
    unsigned char input[256];
    unsigned char output[512];
    memset(input, 'X', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Setup (deflate init, compress, then reset-keep)
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = (uInt)inputLen;
    defstrm.next_out = comp;
    defstrm.avail_out = (uInt)bound;
    deflate(&defstrm, 0);
    deflateResetKeep(&defstrm);
    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = (uInt)inputLen;
    defstrm.next_out = comp + (uInt)defstrm.total_out;
    defstrm.avail_out = (uInt)(bound - (uLong)defstrm.total_out);
    deflate(&defstrm, 0);

    // step 3: Operate (inflate init, reset, inflate, write gz via gzopen64 + gzflush)
    inflateInit_(&instrm, zlibVersion(), (int)sizeof(z_stream));
    inflateReset(&instrm);
    instrm.next_in = comp;
    instrm.avail_in = (uInt)defstrm.total_out;
    instrm.next_out = (Bytef *)output;
    instrm.avail_out = (uInt)sizeof(output);
    inflate(&instrm, 0);
    unsigned int decompressed_len = (unsigned int)instrm.total_out;
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)output, decompressed_len);
    gzflush(gz, 0);
    gzclose(gz);

    // step 4: Validate & Cleanup
    deflateEnd(&defstrm);
    inflateEnd(&instrm);
    free(comp);

    // API sequence test completed successfully
    return 66;
}