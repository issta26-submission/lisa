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
//<ID> 446
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
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLong orig_adler = adler32_z(0UL, (const Bytef *)input, (z_size_t)inputLen);

    // step 2: Initialize deflate and perform compression
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)compBound;
    defStrm.adler = orig_adler;
    deflate(&defStrm, 0);

    // step 3: Initialize inflate, perform decompression, and use gzFile to store compressed data
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)defStrm.total_out;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, 0);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzf, compBuf, (unsigned int)defStrm.total_out);
    gzrewind(gzf);

    // step 4: Validate checksum and cleanup
    uLong recovered_adler = adler32_z(0UL, (const Bytef *)outBuf, (z_size_t)inputLen);
    deflateEnd(&defStrm);
    inflateEnd(&infStrm);
    gzclose(gzf);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}