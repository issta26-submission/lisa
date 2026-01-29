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
//<ID> 490
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream backStrm;
    z_stream defStrm;
    z_stream infStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    uLong compLenU = (uLong)compLen;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);

    // step 2: Initialize / Configure streams and open a gzip file
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzf, compBuf, (unsigned int)compLen);
    gzclose(gzf);

    // step 3: Operate & Validate (decompress for validation and compute checksums)
    uLongf outLen = (uLongf)inputLen;
    uncompress2(outBuf, &outLen, compBuf, &compLenU);
    uLong checksum = adler32(0UL, (const Bytef *)outBuf, (uInt)inputLen);
    uLong crc = crc32(0UL, (const Bytef *)outBuf, (uInt)inputLen);
    uLong validate = checksum ^ crc;
    (void)validate;

    // step 4: Cleanup
    inflateEnd(&infStrm);
    inflateBackEnd(&backStrm);
    deflateEnd(&defStrm);
    free(compBuf);
    free(outBuf);
    free(window);
    return 66; // API sequence test completed successfully
}