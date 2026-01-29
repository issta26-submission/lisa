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
//<ID> 145
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
    unsigned char input[256];
    unsigned char output[512];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Configure (initialize deflater & inflater)
    deflateInit2_(&def_strm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = comp;
    def_strm.avail_out = (uInt)bound;
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate (deflate -> reset-keep -> deflate -> inflate -> write gz -> get tell64)
    deflate(&def_strm, 0);
    deflateResetKeep(&def_strm);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = comp + (uInt)def_strm.total_out;
    def_strm.avail_out = (uInt)(bound - (uLong)def_strm.total_out);
    deflate(&def_strm, 0);
    inf_strm.next_in = comp;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    inf_strm.next_out = (Bytef *)output;
    inf_strm.avail_out = (uInt)sizeof(output);
    inflateUndermine(&inf_strm, 1);
    inflate(&inf_strm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)output, (unsigned int)inf_strm.total_out);
    off64_t pos = gztell64(gz);
    (void)pos;

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, output, (uInt)inf_strm.total_out);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    (void)checksum;
    return 66;
}