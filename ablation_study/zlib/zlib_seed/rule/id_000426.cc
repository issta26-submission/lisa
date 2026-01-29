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
//<ID> 426
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare input buffer and checksum
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[192];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);

    // step 2: Initialize deflate, configure stream pointers and perform deflate
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    defStrm.adler = checksum;
    deflate(&defStrm, 0);

    // step 3: Initialize inflate source, copy the inflate stream and reset the copy, then inflate
    z_stream infSrc;
    memset(&infSrc, 0, sizeof(infSrc));
    inflateInit_(&infSrc, zlibVersion(), (int)sizeof(z_stream));
    infSrc.next_in = compBuf;
    infSrc.avail_in = (uInt)defStrm.total_out;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    infSrc.next_out = outBuf;
    infSrc.avail_out = (uInt)inputLen;
    z_stream infCopy;
    memset(&infCopy, 0, sizeof(infCopy));
    inflateCopy(&infCopy, &infSrc);
    inflateReset(&infCopy);
    inflate(&infCopy, 0);

    // step 4: Cleanup and finalize
    deflateEnd(&defStrm);
    inflateEnd(&infSrc);
    inflateEnd(&infCopy);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}