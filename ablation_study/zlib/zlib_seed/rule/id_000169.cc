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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream inf_strm;
    z_stream inf_copy;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&inf_copy, 0, sizeof(inf_copy));
    unsigned char input[128];
    unsigned char output[256];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup (initialize inflater, copy it, initialize deflater and reset-keep)
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inflateCopy(&inf_copy, &inf_strm);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&def_strm);

    // step 3: Core operations (inflate from compressed buffer, then deflate the result)
    inf_copy.next_in = compBuf;
    inf_copy.avail_in = (uInt)compLen;
    inf_copy.next_out = (Bytef *)output;
    inf_copy.avail_out = (uInt)sizeof(output);
    inflate(&inf_copy, 0);

    uLong recompressBound = compressBound((uLong)inf_copy.total_out);
    Bytef *recompBuf = (Bytef *)malloc((size_t)recompressBound);
    memset(recompBuf, 0, (size_t)recompressBound);
    def_strm.next_in = (Bytef *)output;
    def_strm.avail_in = (uInt)inf_copy.total_out;
    def_strm.next_out = recompBuf;
    def_strm.avail_out = (uInt)recompressBound;
    deflate(&def_strm, 0);

    // step 4: Validate & Cleanup (write recompressed data to gz, rewind, then cleanup)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)recompBuf, (unsigned int)def_strm.total_out);
    gzrewind(gz);
    deflateEnd(&def_strm);
    inflateEnd(&inf_copy);
    inflateEnd(&inf_strm);
    gzclose(gz);
    free(compBuf);
    free(recompBuf);

    // API sequence test completed successfully
    return 66;
}