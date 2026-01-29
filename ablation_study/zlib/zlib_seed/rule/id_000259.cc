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
//<ID> 259
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
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = deflateBound(&def_strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    deflate(&def_strm, 0);
    gzFile gz1 = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz1, compBuf, (unsigned int)def_strm.total_out);
    gzFile gz2 = gzdopen(1, "wb");
    gzwrite(gz2, compBuf, (unsigned int)def_strm.total_out);
    gzclose(gz1);
    gzclose(gz2);

    // step 4: Validate & Cleanup
    inflateUndermine(&inf_strm, 1);
    inflateBackEnd(&inf_strm);
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}