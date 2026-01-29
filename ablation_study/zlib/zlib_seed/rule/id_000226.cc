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
//<ID> 226
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
    memset(input, 'D', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);

    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    uLong compCap = deflateBound(&def_strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compCap);
    memset(compBuf, 0, (size_t)compCap);
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compCap;

    // step 3: Core operations
    deflate(&def_strm, 0);
    deflateResetKeep(&def_strm);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compCap;
    deflate(&def_strm, 0);

    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)inputLen;
    inflate(&inf_strm, 0);
    inflateUndermine(&inf_strm, 1);

    // step 4: Validate & Cleanup
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}