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
//<ID> 1253
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const uLong IN_LEN = 256UL;
    Bytef *input = (Bytef *)malloc((size_t)IN_LEN);
    memset(input, 'A', (size_t)IN_LEN);
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;

    // step 2: Initialize deflate with deflateInit2_, reset keep, and compress
    deflateInit2_(&dstrm, 6, 8, 15, 8, 0, ver, (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)IN_LEN;
    uLong bound = deflateBound(&dstrm, IN_LEN);
    uInt outlen = (uInt)(bound + 16UL);
    Bytef *outbuf = (Bytef *)malloc((size_t)outlen);
    memset(outbuf, 0, (size_t)outlen);
    dstrm.next_out = outbuf;
    dstrm.avail_out = outlen;
    deflate(&dstrm, 4);
    uLong comp_len = dstrm.total_out;

    // step 3: Initialize inflate, validate, and decompress
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = outbuf;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp = (Bytef *)malloc((size_t)(IN_LEN + 16UL));
    memset(decomp, 0, (size_t)(IN_LEN + 16UL));
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)(IN_LEN + 16UL);
    inflateValidate(&istrm, 15);
    inflate(&istrm, 4);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(input);
    free(outbuf);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}