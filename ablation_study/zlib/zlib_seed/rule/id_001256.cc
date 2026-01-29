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
//<ID> 1256
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data buffers and initialize deflate stream
    const uLong IN_LEN = 256UL;
    Bytef *in = (Bytef *)malloc((size_t)IN_LEN);
    memset(in, 'A', (size_t)IN_LEN);
    uLong out_cap = compressBound(IN_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)out_cap);
    memset(comp, 0, (size_t)out_cap);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    const char *ver = zlibVersion();
    deflateInit2_(&dstrm, 6, 8, 15, 8, 0, ver, (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);

    // step 2: Compress the input buffer
    dstrm.next_in = in;
    dstrm.avail_in = (uInt)IN_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)out_cap;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, decompress and validate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)IN_LEN);
    memset(decomp, 0, (size_t)IN_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)IN_LEN;
    inflate(&istrm, 4);
    inflateValidate(&istrm, 1);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(in);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}