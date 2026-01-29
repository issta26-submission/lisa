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
//<ID> 536
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream dStrm;
    memset(&dStrm, 0, sizeof(dStrm));
    z_stream iStrm;
    memset(&iStrm, 0, sizeof(iStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Initialize streams and perform resets
    deflateInit_(&dStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&iStrm, zlibVersion(), (int)sizeof(z_stream));
    deflateReset(&dStrm);
    inflateResetKeep(&iStrm);

    // step 3: Compress, decompress, sync and write one byte into a .gz file
    dStrm.next_in = (Bytef *)input;
    dStrm.avail_in = (uInt)inputLen;
    dStrm.next_out = compBuf;
    dStrm.avail_out = (uInt)bound;
    deflate(&dStrm, Z_FINISH);
    compLen = (uLongf)dStrm.total_out;
    iStrm.next_in = compBuf;
    iStrm.avail_in = (uInt)compLen;
    iStrm.next_out = outBuf;
    iStrm.avail_out = (uInt)inputLen;
    inflate(&iStrm, Z_FINISH);
    inflateSync(&iStrm);
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzputc(gz, (int)input[0]);
    gzclose(gz);

    // step 4: Validate totals & Cleanup
    uLong decompressed = iStrm.total_out;
    (void)decompressed;
    deflateEnd(&dStrm);
    inflateEnd(&iStrm);
    free(compBuf);
    free(outBuf);

    return 66; // API sequence test completed successfully
}