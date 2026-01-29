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
//<ID> 474
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
    z_stream infBackStrm;
    memset(&infBackStrm, 0, sizeof(infBackStrm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    Bytef * dict = (Bytef *)malloc(32);
    memset(dict, 'D', 32);
    uInt dictLen = (uInt)32;
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Compress input using deflate with a preset dictionary
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateSetDictionary(&defStrm, dict, dictLen);
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflate(&defStrm, Z_FINISH);
    compLen = (uLongf)defStrm.total_out;
    deflateEnd(&defStrm);

    // step 3: Initialize inflateBack with window, provide in/out callbacks, run inflateBack, then sync and set dictionary
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&infBackStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
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
        unsigned int avail = (unsigned int)(id->len - id->pos);
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
    inflateBack(&infBackStrm, my_in, (void *)&inDesc, my_out, (void *)&outDesc);
    inflateSyncPoint(&infBackStrm);
    inflateSetDictionary(&infBackStrm, dict, dictLen);

    // step 4: Cleanup and finalize
    inflateBackEnd(&infBackStrm);
    free(compBuf);
    free(outBuf);
    free(window);
    free(dict);
    return 66;
    // API sequence test completed successfully
}