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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream inf_strm;
    z_stream def_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&def_strm, 0, sizeof(def_strm));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress(comp, &compLen, (const Bytef *)input, inputLen);
    deflateInit2_(&def_strm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Setup inflateBack environment
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&inf_strm, 15, window, zlibVersion(), (int)sizeof(z_stream));
    struct MemStream { const Bytef *data; size_t size; size_t pos; Bytef *out; size_t out_pos; };
    MemStream ms;
    ms.data = comp;
    ms.size = (size_t)compLen;
    ms.pos = 0;
    ms.out = (Bytef *)malloc((size_t)inputLen + 1);
    memset(ms.out, 0, (size_t)inputLen + 1);
    ms.out_pos = 0;
    in_func in_cb = [](void *in_desc, unsigned char **buf)->unsigned int {
        MemStream *m = (MemStream *)in_desc;
        unsigned int oldpos = (unsigned int)m->pos;
        *buf = (unsigned char *)(m->data + oldpos);
        m->pos = m->size;
        return (unsigned int)(m->size - oldpos);
    };
    out_func out_cb = [](void *out_desc, unsigned char *buf, unsigned int len)->int {
        MemStream *m = (MemStream *)out_desc;
        memcpy(m->out + m->out_pos, buf, (size_t)len);
        m->out_pos += len;
        return 0;
    };

    // step 3: Operate & Validate
    inflateBack(&inf_strm, in_cb, (void *)&ms, out_cb, (void *)&ms);
    ms.out[ms.out_pos] = '\0';
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)ms.out);

    // step 4: Cleanup
    inflateBackEnd(&inf_strm);
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    gzclose(gz);
    free(comp);
    free(window);
    free(ms.out);

    // API sequence test completed successfully
    return 66;
}