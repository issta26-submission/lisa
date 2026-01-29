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
//<ID> 443
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
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    z_size_t inputLen = (z_size_t)sizeof(input);
    uLong orig_adler = adler32_z(0UL, (const Bytef *)input, inputLen);

    // step 2: Initialize deflate and compress the input
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong compBound = compressBound((uLong)inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)compBound;
    defStrm.adler = orig_adler;
    deflate(&defStrm, 0);

    // step 3: Initialize inflate and decompress into outBuf, validate checksum
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)defStrm.total_out;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, 0);
    uLong recovered_adler = adler32_z(0UL, (const Bytef *)outBuf, inputLen);

    // step 4: Persist compressed data to a gz file, rewind, cleanup
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)defStrm.total_out);
    gzrewind(gz);
    gzclose(gz);
    deflateEnd(&defStrm);
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}