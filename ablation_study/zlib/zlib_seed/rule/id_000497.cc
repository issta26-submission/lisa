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
//<ID> 497
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
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);

    // step 2: Initialize deflate stream and compress input
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflate(&defStrm, Z_FINISH);
    uLong compLen = defStrm.total_out;
    deflateEnd(&defStrm);

    // step 3: Persist compressed data to gzip file and initialize inflateBack then decompress
    gzFile gf = gzopen("test_output.gz", "wb");
    gzwrite(gf, compBuf, (unsigned int)compLen);
    gzclose(gf);
    inflateBackInit_(&backStrm, 15, window, zlibVersion(), (int)sizeof(z_stream));
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
    (void)checksum;

    // step 4: Validate and cleanup
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    inflateEnd(&infStrm);
    inflateBackEnd(&backStrm);
    free(compBuf);
    free(outBuf);
    free(window);
    return 66; // API sequence test completed successfully
}