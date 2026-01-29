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
//<ID> 458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & compress input with deflate
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    unsigned char input[128];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, 0);
    uInt compSize = (uInt)defStrm.total_out;

    // step 2: Initialize inflateBack and make a copy of the stream
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    z_stream copyStrm;
    memset(&copyStrm, 0, sizeof(copyStrm));
    inflateCopy(&copyStrm, &backStrm);

    // step 3: Setup descriptors, callbacks and run inflateBack; compute CRC combine
    struct InDesc { const unsigned char *data; uInt size; uInt pos; };
    struct OutDesc { unsigned char *data; uInt size; uInt pos; };
    InDesc inDesc;
    inDesc.data = compBuf;
    inDesc.size = compSize;
    inDesc.pos = 0;
    uLong outSize = inputLen;
    unsigned char *outBuf = (unsigned char *)malloc((size_t)outSize);
    memset(outBuf, 0, (size_t)outSize);
    OutDesc outDesc;
    outDesc.data = outBuf;
    outDesc.size = (uInt)outSize;
    outDesc.pos = 0;
    unsigned int (*in_cb)(void *, unsigned char **) = +[](void *d, unsigned char **buf) -> unsigned int {
        InDesc *id = (InDesc *)d;
        unsigned int avail = id->size - id->pos;
        *buf = (unsigned char *)(id->data + id->pos);
        id->pos += avail;
        return avail;
    };
    int (*out_cb)(void *, unsigned char *, unsigned int) = +[](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        memcpy(od->data + od->pos, buf, len);
        od->pos += len;
        return 0;
    };
    inflateBack(&backStrm, (unsigned int (*)(void *, unsigned char **))in_cb, &inDesc, (int (*)(void *, unsigned char *, unsigned int))out_cb, &outDesc);
    uLong crc1 = crc32_z(0UL, (const Bytef *)input, (z_size_t)inputLen);
    uLong crc2 = crc32_z(0UL, (const Bytef *)outBuf, (z_size_t)outDesc.pos);
    uLong combined = crc32_combine64(crc1, crc2, (off64_t)inputLen);
    (void)combined;

    // step 4: Cleanup and finalize
    inflateEnd(&copyStrm);
    inflateBackEnd(&backStrm);
    deflateEnd(&defStrm);
    free(compBuf);
    free(outBuf);
    free(window);
    return 66;
    // API sequence test completed successfully
}