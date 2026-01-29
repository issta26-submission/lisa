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
//<ID> 135
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
    unsigned int outSize = 512;
    unsigned char *output = (unsigned char *)malloc((size_t)outSize);
    memset(input, 'Z', sizeof(input));
    memset(output, 0, outSize);
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Setup / Configure (deflate init, compress, reset-keep, compress again)
    deflateInit_(&def_stream, 6, zlibVersion(), (int)sizeof(z_stream));
    def_stream.next_in = (Bytef *)input;
    def_stream.avail_in = (uInt)inputLen;
    def_stream.next_out = comp;
    def_stream.avail_out = (uInt)bound;
    deflate(&def_stream, 0);
    deflateResetKeep(&def_stream);
    def_stream.next_in = (Bytef *)input;
    def_stream.avail_in = (uInt)inputLen;
    def_stream.next_out = comp;
    def_stream.avail_out = (uInt)bound;
    deflate(&def_stream, 0);
    uLong compLen = def_stream.total_out;

    // step 3: Operate & Validate (inflate, reset, inflate again, write gz and flush)
    inflateInit_(&inf_stream, zlibVersion(), (int)sizeof(z_stream));
    inf_stream.next_in = comp;
    inf_stream.avail_in = (uInt)compLen;
    inf_stream.next_out = output;
    inf_stream.avail_out = (uInt)outSize;
    inflate(&inf_stream, 0);
    inflateReset(&inf_stream);
    inf_stream.next_in = comp;
    inf_stream.avail_in = (uInt)compLen;
    inf_stream.next_out = output;
    inf_stream.avail_out = (uInt)outSize;
    inflate(&inf_stream, 0);
    unsigned int decomp_len = (unsigned int)inf_stream.total_out;
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)output, decomp_len);
    gzflush(gz, 0);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&def_stream);
    inflateEnd(&inf_stream);
    free(comp);
    free(output);

    // API sequence test completed successfully
    return 66;
}