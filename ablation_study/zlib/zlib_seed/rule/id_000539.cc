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
//<ID> 539
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Initialize & configure deflate, then perform compression
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateReset(&defStrm);
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    uLongf compProduced = (uLongf)(bound - defStrm.avail_out);
    deflateEnd(&defStrm);

    // step 3: Persist compressed data to a .gz file (use gzfwrite and gzputc)
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzfwrite((voidpc)compBuf, (z_size_t)1, (z_size_t)compProduced, gz);
    gzputc(gz, 'X');
    gzclose(gz);

    // step 4: Initialize inflate, use inflateResetKeep and inflateSync, then inflate & cleanup
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateResetKeep(&infStrm);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compProduced;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflateSync(&infStrm);
    inflate(&infStrm, Z_FINISH);
    uLong decompressed = infStrm.total_out;
    (void)decompressed;
    inflateEnd(&infStrm);

    free(compBuf);
    free(outBuf);

    return 66; // API sequence test completed successfully
}