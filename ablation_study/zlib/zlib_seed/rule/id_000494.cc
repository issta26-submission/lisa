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
//<ID> 494
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream backStrm;
    z_stream infStrm;
    z_stream defStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    memset(&infStrm, 0, sizeof(infStrm));
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[256];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);

    // step 2: Initialize / Configure streams
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate & Validate (decompress with inflateBack, checksum, write to gz)
    struct InDesc { const Bytef * data; uInt len; uInt pos; };
    struct OutDesc { Bytef * data; uInt pos; };
    InDesc inDesc;
    inDesc.data = compBuf;
    inDesc.len = (uInt)compLen;
    inDesc.pos = 0;
    OutDesc outDesc;
    outDesc.data = outBuf;
    outDesc.pos = 0;
    in_func my_in = [](void * d, unsigned char ** buf)->unsigned int {
        InDesc * id = (InDesc *)d;
        unsigned int avail = id->len - id->pos;
        *buf = (unsigned char *)(id->data + id->pos);
        id->pos += avail;
        return avail;
    };
    out_func my_out = [](void * d, unsigned char * buf, unsigned int len)->int {
        OutDesc * od = (OutDesc *)d;
        memcpy(od->data + od->pos, buf, (size_t)len);
        od->pos += len;
        return 0;
    };
    inflateBack(&backStrm, my_in, (void *)&inDesc, my_out, (void *)&outDesc);
    uLong checksum = adler32(0UL, (const Bytef *)outBuf, (uInt)inputLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)inputLen);

    // step 4: Cleanup
    inflateBackEnd(&backStrm);
    deflateEnd(&defStrm);
    inflateEnd(&infStrm);
    gzclose(gz);
    free(compBuf);
    free(outBuf);
    free(window);

    // API sequence test completed successfully
    return 66;
}