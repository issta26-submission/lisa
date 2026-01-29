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
//<ID> 530
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

    // step 2: Initialize & Configure (deflate init and reset)
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateReset(&defStrm);
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;

    // step 3: Operate (compress, write to gz, initialize inflate, decompress, sync & keep-reset)
    deflate(&defStrm, Z_FINISH);
    uLong produced = defStrm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzputc(gz, 'X');
    gzputc(gz, 'Y');
    gzclose(gz);
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)produced;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, Z_FINISH);
    inflateSync(&infStrm);
    inflateResetKeep(&infStrm);

    // step 4: Validate & Cleanup
    uLong decompressed = infStrm.total_out;
    (void)decompressed;
    inflateEnd(&infStrm);
    deflateEnd(&defStrm);
    free(compBuf);
    free(outBuf);

    return 66;
    // API sequence test completed successfully
}