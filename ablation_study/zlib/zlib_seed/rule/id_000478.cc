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
//<ID> 478
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
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
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Prepare in/out descriptors and run inflateBack, then use inflateSyncPoint and inflateSetDictionary
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
    inflateSyncPoint(&backStrm);
    inflateSetDictionary(&backStrm, (const Bytef *)outDesc.data, (uInt)outDesc.pos);

    // step 4: Validate checksum, cleanup and finish
    uLong checksum = crc32_z(0UL, (const Bytef *)outBuf, (z_size_t)outDesc.pos);
    (void)checksum;
    inflateBackEnd(&backStrm);
    free(compBuf);
    free(outBuf);
    free(window);
    return 66; // API sequence test completed successfully
}