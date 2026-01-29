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
//<ID> 325
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;

    // step 2: Configure compression buffers
    uLong bound = deflateBound(&dstrm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Operate & Validate (compress then prepare inflate and validate/prime/mark)
    deflate(&dstrm, 4);
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    Bytef * outBuf = (Bytef *)malloc(256);
    memset(outBuf, 0, 256);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)256;
    inflatePrime(&istrm, 0, 0);
    int valid = inflateValidate(&istrm, 1);
    inflate(&istrm, 4);
    long mark = inflateMark(&istrm);
    (void)valid;
    (void)mark;

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}