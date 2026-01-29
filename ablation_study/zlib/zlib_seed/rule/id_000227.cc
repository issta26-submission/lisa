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
//<ID> 227
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
    unsigned char input[512];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);

    // step 2: Setup (initialize compressors/decompressors and prepare bounds)
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&def_strm);
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compBufCap = compBound;

    // step 3: Core operations (compress then decompress, using inflateUndermine)
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBufCap;
    deflate(&def_strm, 0);
    uLong compSize = def_strm.total_out;

    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compSize;
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    uLong outBufCap = inputLen;
    memset(outBuf, 0, (size_t)outBufCap);
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)outBufCap;
    inflateUndermine(&inf_strm, 1);
    inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}