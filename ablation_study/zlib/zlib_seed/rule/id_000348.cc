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
//<ID> 348
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize (prepare input and deflate)
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong compLenUL = dstrm.total_out;
    unsigned int compLen = (unsigned int)compLenUL;
    deflateEnd(&dstrm);

    // step 2: Initialize inflate with inflateInit2_ and decompress
    inflateInit2_(&istrm, 15, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)inputLen;
    inflate(&istrm, 0);

    // step 3: Write compressed data to a gz file, inspect offset and close
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, compLen);
    off64_t offset = gzoffset64(gz);
    (void)offset;
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}