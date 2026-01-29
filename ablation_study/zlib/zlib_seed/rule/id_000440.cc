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
//<ID> 440
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
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    unsigned char readBuf[256];
    memset(readBuf, 0, sizeof(readBuf));

    // step 2: Write input to a gz file, close and reopen for reading, then rewind
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gzf, (voidpc)input, (unsigned int)inputLen);
    gzclose(gzf);
    gzf = gzopen("test_zlib_api_sequence.gz", "rb");
    gzrewind(gzf);
    gzread(gzf, (voidp)readBuf, (unsigned int)inputLen);
    uLong read_adler = adler32_z(0UL, (const Bytef *)readBuf, (z_size_t)inputLen);

    // step 3: Initialize deflate, perform deflate, initialize inflate and perform inflate
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, 0);
    deflateEnd(&defStrm);
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)defStrm.total_out;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;
    inflate(&infStrm, 0);
    inflateEnd(&infStrm);
    uLong recovered_adler = adler32_z(0UL, (const Bytef *)outBuf, (z_size_t)inputLen);

    // step 4: Cleanup and finalize
    gzclose(gzf);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}