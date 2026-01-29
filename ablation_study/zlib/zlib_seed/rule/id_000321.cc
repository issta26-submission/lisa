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
//<ID> 321
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defl;
    z_stream infl;
    memset(&defl, 0, sizeof(defl));
    memset(&infl, 0, sizeof(infl));
    deflateInit_(&defl, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infl, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = deflateBound(&defl, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defl.next_in = (Bytef *)input;
    defl.avail_in = (uInt)inputLen;
    defl.next_out = compBuf;
    defl.avail_out = (uInt)bound;
    Bytef * outBuf = (Bytef *)malloc( (size_t)inputLen );
    memset(outBuf, 0, (size_t)inputLen);

    // step 3: Operate & Validate
    deflate(&defl, 4);
    infl.next_in = compBuf;
    infl.avail_in = (uInt)defl.total_out;
    infl.next_out = outBuf;
    infl.avail_out = (uInt)inputLen;
    inflatePrime(&infl, 1, 1);
    inflate(&infl, 0);
    inflateValidate(&infl, 1);
    long markval = inflateMark(&infl);
    (void)markval;

    // step 4: Cleanup
    deflateEnd(&defl);
    inflateEnd(&infl);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}