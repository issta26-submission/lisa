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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream istrm;
    z_stream istrm_copy;
    z_stream dstrm;
    z_stream outstrm;
    unsigned char input[128];
    unsigned char decomp[256];
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&outstrm, 0, sizeof(outstrm));
    memset(input, 'A', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)cb;

    // step 2: Configure
    deflateResetKeep(&dstrm);
    inflateCopy(&istrm_copy, &istrm);

    // step 3: Operate & Validate
    deflate(&dstrm, 0);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    outstrm.next_out = decomp;
    outstrm.avail_out = (uInt)sizeof(decomp);
    outstrm.total_out = 0;
    unsigned int (*in_cb)(void *, unsigned char **) = [](void *desc, unsigned char **buf) -> unsigned int {
        z_stream *s = (z_stream *)desc;
        unsigned int n = s->avail_in;
        *buf = s->next_in;
        s->next_in += n;
        s->avail_in = 0;
        return n;
    };
    int (*out_cb)(void *, unsigned char *, unsigned int) = [](void *desc, unsigned char *buf, unsigned int len) -> int {
        z_stream *o = (z_stream *)desc;
        unsigned int tocopy = len;
        tocopy = (o->avail_out < tocopy) ? o->avail_out : tocopy;
        memcpy(o->next_out, buf, (size_t)tocopy);
        o->next_out += tocopy;
        o->avail_out -= tocopy;
        o->total_out += tocopy;
        return 0;
    };
    inflateBack(&istrm, in_cb, (void *)&istrm, out_cb, (void *)&outstrm);
    inflateBackEnd(&istrm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, decomp, (unsigned int)outstrm.total_out);
    gzflush(gz, 0);

    // step 4: Cleanup
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);
    deflateEnd(&dstrm);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}