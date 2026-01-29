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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    const char src_text[] = "Hello zlib!"; /* includes terminating NUL */
    uLong src_len = (uLong)(sizeof(src_text)); /* include terminating byte */
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;

    // step 2: Compress source (simple one-shot)
    compress(comp_buf, &comp_len, (const Bytef *)src_text, src_len);

    // step 3: Configure (deflater init) and Setup inflateBack callbacks and state
    deflateInit2_(&defstrm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));

    struct InState { Bytef *data; uLong len; uLong pos; };
    struct OutState { Bytef *out; uLong cap; uLong pos; };
    InState in_state;
    OutState out_state;
    in_state.data = comp_buf;
    in_state.len = comp_len;
    in_state.pos = 0;
    out_state.cap = src_len;
    out_state.out = (Bytef *)malloc((size_t)out_state.cap);
    memset(out_state.out, 0, (size_t)out_state.cap);
    out_state.pos = 0;

    in_func in_cb = [](void *desc, unsigned char **buf)->unsigned int {
        InState *s = (InState *)desc;
        *buf = s->data + s->pos;
        unsigned int avail = (unsigned int)(s->len - s->pos);
        s->pos = s->len;
        return avail;
    };
    out_func out_cb = [](void *desc, unsigned char *data, unsigned int len)->int {
        OutState *o = (OutState *)desc;
        memcpy(o->out + o->pos, data, (size_t)len);
        o->pos += (uLong)len;
        return 0;
    };

    inflateInit2_(&infstrm, 15, zlibVersion(), (int)sizeof(z_stream));
    inflateBack(&infstrm, in_cb, (void *)&in_state, out_cb, (void *)&out_state);

    // step 4: Write decompressed data to gz file and Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence_output.gz", "wb");
    gzputs(gz, "Decompressed: ");
    gzputs(gz, (const char *)out_state.out);
    gzclose(gz);
    inflateEnd(&infstrm);
    deflateEnd(&defstrm);
    free(comp_buf);
    free(out_state.out);

    // API sequence test completed successfully
    return 66;
}