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
//<ID> 153
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
    unsigned char input[256];
    unsigned char output[512];
    memset(input, 'Z', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Setup (compress input with deflate, prepare inflater and descriptors)
    def_strm.zalloc = (alloc_func)0;
    def_strm.zfree = (free_func)0;
    def_strm.opaque = (voidpf)0;
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = comp;
    def_strm.avail_out = (uInt)bound;
    deflate(&def_strm, 0);
    deflateEnd(&def_strm);
    inf_strm.zalloc = (alloc_func)0;
    inf_strm.zfree = (free_func)0;
    inf_strm.opaque = (voidpf)0;
    inflateInit2_(&inf_strm, 15, zlibVersion(), (int)sizeof(z_stream));
    uLong compiled_flags = zlibCompileFlags();

    // step 3: Core operations (inflateBack using callbacks to feed compressed data and collect output)
    struct InDesc { unsigned char *data; uLong size; uLong pos; };
    struct OutDesc { unsigned char *buf; uLong size; uLong pos; };
    InDesc in_desc;
    in_desc.data = (unsigned char *)comp;
    in_desc.size = (uLong)def_strm.total_out;
    in_desc.pos = 0;
    OutDesc out_desc;
    out_desc.buf = output;
    out_desc.size = (uLong)sizeof(output);
    out_desc.pos = 0;
    in_func in_cb = +[](void *d, unsigned char **buf)->unsigned int {
        InDesc *id = (InDesc *)d;
        unsigned int rem = (unsigned int)(id->size - id->pos);
        *buf = id->data + id->pos;
        id->pos += (uLong)rem;
        return rem;
    };
    out_func out_cb = +[](void *d, unsigned char *buf, unsigned int len)->int {
        OutDesc *od = (OutDesc *)d;
        memcpy(od->buf + od->pos, buf, (size_t)len);
        od->pos += (uLong)len;
        return 0;
    };
    inflateBack(&inf_strm, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, output, (uInt)out_desc.pos);
    (void)checksum;
    (void)compiled_flags;
    inflateEnd(&inf_strm);
    free(comp);
    // API sequence test completed successfully
    return 66;
}