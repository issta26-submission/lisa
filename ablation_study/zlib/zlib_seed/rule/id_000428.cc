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
//<ID> 428
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Prepare input and buffers
    z_stream defStrm;
    z_stream infSrc;
    z_stream infCopy;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&infSrc, 0, sizeof(infSrc));
    memset(&infCopy, 0, sizeof(infCopy));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Initialize deflate and configure stream pointers
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    defStrm.adler = checksum;

    // step 3: Operate (deflate) then initialize inflate, copy and reset
    deflate(&defStrm, 0);
    inflateInit_(&infSrc, zlibVersion(), (int)sizeof(z_stream));
    infSrc.next_in = compBuf;
    infSrc.avail_in = (uInt)defStrm.total_out;
    infSrc.next_out = outBuf;
    infSrc.avail_out = (uInt)inputLen;
    inflateCopy(&infCopy, &infSrc);
    inflate(&infCopy, 0);
    inflateReset(&infSrc);

    // step 4: Cleanup and finalize
    deflateEnd(&defStrm);
    inflateEnd(&infSrc);
    inflateEnd(&infCopy);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}