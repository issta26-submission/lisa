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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    unsigned char plain[128];
    memset(plain, 'A', sizeof(plain));
    uLong sourceLen = (uLong)sizeof(plain);
    uLong cb = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)cb);
    memset(comp, 0, (size_t)cb);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&istrm_copy, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    dstrm.next_in = plain;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)cb;
    deflate(&dstrm, 0);
    deflateResetKeep(&dstrm);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = plain;
    istrm.avail_out = (uInt)sourceLen;
    inflateCopy(&istrm_copy, &istrm);

    // step 3: Operate & Validate
    struct InDesc { unsigned char *data; uLong size; uLong pos; };
    InDesc in_desc = { comp, (uLong)dstrm.total_out, 0 };
    auto in_lambda = [](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        if (d->pos >= d->size) return 0;
        *buf = d->data + (size_t)d->pos;
        unsigned ret = (unsigned)(d->size - d->pos);
        d->pos = d->size;
        return ret;
    };
    auto out_lambda = [](void *desc, unsigned char *buf, unsigned int len) -> int {
        gzFile gz_local = (gzFile)desc;
        gzwrite(gz_local, buf, len);
        return 0;
    };
    in_func in_cb = (in_func)in_lambda;
    out_func out_cb = (out_func)out_lambda;
    inflateBack(&istrm, in_cb, &in_desc, out_cb, (void *)gz);
    inflateBackEnd(&istrm);
    gzflush(gz, 0);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}