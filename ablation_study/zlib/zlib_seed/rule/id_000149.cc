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
//<ID> 149
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
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[128];
    unsigned char output[256];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);

    // step 2: Setup (initialize deflate with deflateInit2_ and configure streams)
    deflateInit2_(&def_strm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)bound;
    deflate(&def_strm, 0);

    // step 3: Operate (reset-keep, continue deflate, inflate, and call inflateUndermine)
    deflateResetKeep(&def_strm);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf + (uInt)def_strm.total_out;
    def_strm.avail_out = (uInt)(bound - (uLong)def_strm.total_out);
    deflate(&def_strm, 0);
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    inf_strm.next_out = (Bytef *)output;
    inf_strm.avail_out = (uInt)sizeof(output);
    inflate(&inf_strm, 0);
    inflateUndermine(&inf_strm, 1);

    // step 4: Validate & Cleanup (write out via gzopen, query offset with gztell64, then cleanup)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)output, (unsigned int)inf_strm.total_out);
    off64_t pos = gztell64(gz);
    (void)pos;
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    gzclose(gz);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}