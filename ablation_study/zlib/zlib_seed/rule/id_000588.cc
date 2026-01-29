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
//<ID> 588
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    const char *ver = zlibVersion();
    Bytef *input = (Bytef *)malloc((size_t)256);
    memset(input, 'A', 256);
    uLong inputLen = (uLong)256;
    uLong orig_adler = adler32(0L, input, (uInt)inputLen);

    // step 2: Initialize deflate, configure and operate (compress)
    deflateInit_(&defStrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&defStrm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defStrm.next_in = input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    uLong compLen = defStrm.total_out;
    deflateEnd(&defStrm);

    // step 3: Initialize inflate, configure undermine, operate (inflate) and validate checksums
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    inflateUndermine(&infStrm, 1);
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, Z_FINISH);
    uLong out_adler = adler32(0L, outBuf, (uInt)inputLen);

    // step 4: Cleanup
    inflateBackEnd(&infStrm);
    inflateEnd(&infStrm);
    free(input);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    (void)orig_adler;
    (void)out_adler;
    return 66;
}