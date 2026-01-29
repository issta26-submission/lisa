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
//<ID> 447
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
    uLong inputLen = (uLong)sizeof(input);
    uLong orig_adler = adler32_z(0UL, (const Bytef *)input, (z_size_t)inputLen);
    uLong compBound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    Bytef * readBuf = (Bytef *)malloc((size_t)compBound);
    memset(readBuf, 0, (size_t)compBound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Initialize deflate, configure stream and compress
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)compBound;
    deflate(&defStrm, 0);
    uLong compSize = defStrm.total_out;

    // step 3: Write compressed data to a gzip file and rewind for reading
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compSize);
    gzrewind(gz);
    int bytesRead = gzread(gz, readBuf, (unsigned int)compSize);

    // step 4: Initialize inflate, decompress, validate and cleanup
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = readBuf;
    infStrm.avail_in = (uInt)bytesRead;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, 0);
    uLong recovered_adler = adler32_z(0UL, (const Bytef *)outBuf, (z_size_t)inputLen);
    int result_match = (recovered_adler == orig_adler) ? 1 : 0;
    (void)result_match;
    deflateEnd(&defStrm);
    inflateEnd(&infStrm);
    gzclose(gz);
    free(compBuf);
    free(readBuf);
    free(outBuf);
    return 66; // API sequence test completed successfully
}