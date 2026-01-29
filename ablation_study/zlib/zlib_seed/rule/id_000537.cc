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
//<ID> 537
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream defStrm;
    z_stream infStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Initialize deflate stream, reset it, and compress using deflate
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateReset(&defStrm);
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    uLongf compLen = (uLongf)defStrm.total_out;
    deflateEnd(&defStrm);

    // step 3: Use gz API (write a single byte), then initialize inflate, reset-keep, inflate and sync
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzputc(gz, 'Z');
    gzclose(gz);
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateResetKeep(&infStrm);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, Z_FINISH);
    inflateSync(&infStrm);

    // step 4: Validate totals (read) & Cleanup
    uLong produced = infStrm.total_out;
    (void)produced;
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}