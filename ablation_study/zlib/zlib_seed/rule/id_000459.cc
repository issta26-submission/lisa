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
//<ID> 459
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream infBackStrm;
    memset(&infBackStrm, 0, sizeof(infBackStrm));
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(copyStrm));
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[256];
    memset(input, 'P', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Compress input and initialize inflateBack with window
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&infBackStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Prepare in/out descriptors and run inflateCopy + inflateBack, then combine CRCs
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
    inflateCopy(&copyStrm, &infBackStrm);
    inflateBack(&copyStrm, my_in, (void *)&inDesc, my_out, (void *)&outDesc);
    uInt half = outDesc.pos / 2;
    uLong crc1 = crc32_z(0UL, (const Bytef *)outDesc.data, (z_size_t)half);
    uLong crc2 = crc32_z(0UL, (const Bytef *)(outDesc.data + half), (z_size_t)(outDesc.pos - half));
    uLong combined = crc32_combine64(crc1, crc2, (off64_t)half);
    (void)combined;

    // step 4: Initialize and immediately end a deflate stream, cleanup buffers
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateEnd(&defStrm);
    free(compBuf);
    free(outBuf);
    free(window);
    return 66;
    // API sequence test completed successfully
}