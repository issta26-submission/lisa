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
//<ID> 30
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
    z_stream copystrm;
    unsigned char input[128];
    unsigned char decomp[256];
    uLong sourceLen;
    uLong cb;
    unsigned char *comp;
    gzFile gz;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(&copystrm, 0, sizeof(copystrm));
    memset(input, 'A', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    sourceLen = (uLong)sizeof(input);
    cb = compressBound(sourceLen);
    comp = (unsigned char *)malloc((size_t)cb);
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)cb;
    deflateResetKeep(&dstrm);
    deflate(&dstrm, 0);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)sizeof(decomp);
    inflateCopy(&copystrm, &istrm);
    gzwrite(gz, comp, (unsigned int)dstrm.total_out);
    gzflush(gz, 0);

    // step 3: Operate & Validate
    struct InDesc { unsigned char *data; uInt size; uInt pos; };
    struct OutDesc { unsigned char *buf; uInt size; uInt pos; };
    InDesc *ind = (InDesc *)malloc(sizeof(InDesc));
    OutDesc *outd = (OutDesc *)malloc(sizeof(OutDesc));
    ind->data = comp;
    ind->size = (uInt)dstrm.total_out;
    ind->pos = 0;
    outd->buf = decomp;
    outd->size = (uInt)sizeof(decomp);
    outd->pos = 0;
    in_func inf = (in_func) +[](void *desc, unsigned char **buf) -> unsigned int {
        InDesc *d = (InDesc *)desc;
        unsigned int avail = d->size - d->pos;
        *buf = d->data + d->pos;
        d->pos = d->size;
        return avail;
    };
    out_func outf = (out_func) +[](void *desc, unsigned char *buf, unsigned int len) -> int {
        OutDesc *o = (OutDesc *)desc;
        memcpy(o->buf + o->pos, buf, (size_t)len);
        o->pos += len;
        return (int)len;
    };
    inflateBack(&istrm, inf, (void *)ind, outf, (void *)outd);
    inflateBackEnd(&istrm);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    inflateEnd(&copystrm);
    gzclose(gz);
    free(comp);
    free(ind);
    free(outd);

    // API sequence test completed successfully
    return 66;
}