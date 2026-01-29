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
//<ID> 161
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
    memset(compBuf, 0, (size_t)compBound);

    // step 2: Setup (initialize deflate, perform initial compression)
    deflateInit2_(&def_strm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    deflate(&def_strm, 0);

    // step 3: Operate (reset-keep, compress more, initialize inflater, inflate, copy inflater, write to gz and rewind)
    deflateResetKeep(&def_strm);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf + (uInt)def_strm.total_out;
    def_strm.avail_out = (uInt)(compBound - (uLong)def_strm.total_out);
    deflate(&def_strm, 0);
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    inf_strm.next_out = (Bytef *)output;
    inf_strm.avail_out = (uInt)sizeof(output);
    inflate(&inf_strm, 0);
    inflateCopy(&inf_copy, &inf_strm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)output, (unsigned int)inf_strm.total_out);
    gzrewind(gz);

    // step 4: Validate & Cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    inflateEnd(&inf_copy);
    gzclose(gz);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}