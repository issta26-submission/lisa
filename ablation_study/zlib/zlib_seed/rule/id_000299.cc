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
//<ID> 299
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&inf_strm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    deflate(&def_strm, 4);
    deflateResetKeep(&def_strm);
    uInt compLen = (uInt)def_strm.total_out;
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    inflateSetDictionary(&inf_strm, (const Bytef *)input, (uInt)32);

    // step 3: Operate & Validate
    struct InDesc { Bytef * data; uInt len; uInt pos; };
    struct OutDesc { Bytef * data; uInt len; uInt pos; };
    InDesc in_desc = { compBuf, compLen, 0 };
    unsigned char outBuf[256];
    memset(outBuf, 0, sizeof(outBuf));
    OutDesc out_desc = { outBuf, (uInt)sizeof(outBuf), 0 };
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
    inflateBack(&inf_strm, in_cb, &in_desc, out_cb, &out_desc);

    // step 4: Cleanup
    inflateBackEnd(&inf_strm);
    deflateEnd(&def_strm);
    free(window);
    free(compBuf);
    return 66;
    // API sequence test completed successfully
}