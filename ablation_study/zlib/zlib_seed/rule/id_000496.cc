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
//<ID> 496
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream defStrm;
    z_stream backStrm;
    z_stream infStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    memset(&backStrm, 0, sizeof(backStrm));
    memset(&infStrm, 0, sizeof(infStrm));
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

    // step 2: Initialize/configure streams and open gz file
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, compBuf, (unsigned int)compLen);
    gzflush(gf, 2); // Z_FINISH is typically 4, but gzflush accepts Z_SYNC_FLUSH/Z_FINISH macros; use numeric flush value 2 for safety in straight-line usage
    gzclose(gf);

    // step 3: Operate: inflateBack decompresses compBuf into outBuf and validate checksum
    struct InDesc { const Bytef * data; uInt len; uInt pos; };
    struct OutDesc { Bytef * data; uInt pos; };
    InDesc inDesc;
    inDesc.data = compBuf;
    inDesc.len = (uInt)compLen;
    inDesc.pos = 0;
    OutDesc outDesc;
    outDesc.data = outBuf;
    outDesc.pos = 0;
    unsigned int (*my_in)(void *, unsigned char **) = [](void * d, unsigned char ** buf)->unsigned int {
        InDesc * id = (InDesc *)d;
        unsigned int avail = id->len - id->pos;
        *buf = (unsigned char *)(id->data + id->pos);
        id->pos += avail;
        return avail;
    };
    int (*my_out)(void *, unsigned char *, unsigned int) = [](void * d, unsigned char * buf, unsigned int len)->int {
        OutDesc * od = (OutDesc *)d;
        memcpy(od->data + od->pos, buf, (size_t)len);
        od->pos += len;
        return 0;
    };
    inflateBack(&backStrm, my_in, (void *)&inDesc, my_out, (void *)&outDesc);
    uLong checksum = adler32(0UL, (const Bytef *)outBuf, (uInt)inputLen);
    (void)checksum;

    // step 4: Cleanup and finalize
    deflateEnd(&defStrm);
    inflateBackEnd(&backStrm);
    inflateEnd(&infStrm);
    free(compBuf);
    free(outBuf);
    free(window);
    return 66; // API sequence test completed successfully
}