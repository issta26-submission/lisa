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
//<ID> 464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    memset(compBuf, 0, (size_t)bound);

    // step 2: Compress input and write to a gz file, then reopen and read one byte
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)input, (unsigned int)inputLen);
    gzclose(gz);
    gz = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_byte = gzgetc_(gz);
    (void)first_byte;
    gzclose(gz);

    // step 3: Initialize inflate, decompress compressed buffer, validate checksum, reset keep
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, 0);
    uLong crc_val = crc32(0UL, outBuf, (uInt)inputLen);
    (void)crc_val;
    inflateResetKeep(&infStrm);

    // step 4: Initialize inflateBack stream and immediately clean it up, then final cleanup
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateBackEnd(&backStrm);
    inflateEnd(&infStrm);
    free(window);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}