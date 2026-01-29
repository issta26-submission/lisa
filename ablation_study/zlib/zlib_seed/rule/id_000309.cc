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
//<ID> 309
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream inf_back;
    z_stream inf_std;
    memset(&inf_back, 0, sizeof(inf_back));
    memset(&inf_std, 0, sizeof(inf_std));
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&inf_back, 15, window, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_std, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    uLongf compLen = (uLongf)compBound;
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);
    struct InDesc { Bytef * data; uInt len; uInt pos; };
    struct OutDesc { Bytef * data; uInt len; uInt pos; };
    InDesc in_desc = { compBuf, (uInt)compLen, 0 };
    Bytef * outBuf = (Bytef *)malloc(256);
    memset(outBuf, 0, 256);
    OutDesc out_desc = { outBuf, (uInt)256, 0 };
    in_func in_cb = [](void * d, unsigned char ** buf) -> unsigned int {
        InDesc * id = (InDesc *)d;
        unsigned int avail = (id->len > id->pos) ? (id->len - id->pos) : 0u;
        *buf = avail ? (unsigned char *)(id->data + id->pos) : (unsigned char *)0;
        id->pos += avail;
        return avail;
    };
    out_func out_cb = [](void * d, unsigned char * buf, unsigned int len) -> int {
        OutDesc * od = (OutDesc *)d;
        unsigned int room = (od->len > od->pos) ? (od->len - od->pos) : 0u;
        unsigned int copy = (len <= room) ? len : room;
        memcpy(od->data + od->pos, buf, (size_t)copy);
        od->pos += copy;
        return 0;
    };

    // step 3: Operate & Validate
    inflateUndermine(&inf_back, 1);
    inflateReset(&inf_std);
    inflateBack(&inf_back, in_cb, &in_desc, out_cb, &out_desc);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, "zlib sequence\n", 14);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&inf_std);
    inflateBackEnd(&inf_back);
    free(window);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}