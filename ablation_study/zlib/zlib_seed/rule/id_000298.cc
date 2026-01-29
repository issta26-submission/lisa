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
//<ID> 298
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
    unsigned char dictionary[32];
    unsigned char outBuf[256];
    memset(input, 'A', sizeof(input));
    memset(dictionary, 'D', sizeof(dictionary));
    memset(outBuf, 0, sizeof(outBuf));
    uLong inputLen = (uLong)sizeof(input);
    uInt dictLen = (uInt)sizeof(dictionary);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    uLong dictAdler = adler32(0UL, dictionary, dictLen);
    (void)dictAdler;
    deflateResetKeep(&def_strm);

    // step 3: Operate & Validate
    deflate(&def_strm, 4);
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)sizeof(outBuf);
    inflateSetDictionary(&inf_strm, dictionary, dictLen);
    inflateBack(&inf_strm, (in_func)0, (void *)0, (out_func)0, (void *)0);

    // step 4: Cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    return 66;
    // API sequence test completed successfully
}