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
//<ID> 429
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and checksum
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream infSrc;
    memset(&infSrc, 0, sizeof(infSrc));
    z_stream infCopy;
    memset(&infCopy, 0, sizeof(infCopy));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong orig_adler = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong compBound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Initialize deflate, configure stream and perform a deflate call
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)compBound;
    defStrm.adler = orig_adler;
    deflate(&defStrm, 0);

    // step 3: Initialize inflate, copy stream, reset original inflate and inflate from copy
    inflateInit_(&infSrc, zlibVersion(), (int)sizeof(z_stream));
    infSrc.next_in = compBuf;
    infSrc.avail_in = (uInt)defStrm.total_out;
    infSrc.next_out = outBuf;
    infSrc.avail_out = (uInt)inputLen;
    inflateCopy(&infCopy, &infSrc);
    inflateReset(&infSrc);
    inflate(&infCopy, 0);
    uLong recovered_adler = adler32(0UL, (const Bytef *)outBuf, (uInt)inputLen);

    // step 4: Cleanup and finalize
    deflateEnd(&defStrm);
    inflateEnd(&infSrc);
    inflateEnd(&infCopy);
    free(compBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}