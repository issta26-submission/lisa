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
//<ID> 292
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
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    Bytef dict[32];
    memset(dict, 'D', sizeof(dict));
    uInt dictLen = (uInt)sizeof(dict);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;

    // step 3: Operate & Validate
    deflate(&def_strm, 4);
    uLong compSize = def_strm.total_out;
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    (void)checksum;
    deflateResetKeep(&def_strm);

    // prepare inflate stream to consume the compressed data
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compSize;
    unsigned char outBuf[256];
    memset(outBuf, 0, sizeof(outBuf));
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)sizeof(outBuf);

    // attempt to set dictionary (no branching on return) and inflate
    inflateSetDictionary(&inf_strm, dict, dictLen);
    inflate(&inf_strm, 4);

    // step 4: Cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);

    return 66;
}