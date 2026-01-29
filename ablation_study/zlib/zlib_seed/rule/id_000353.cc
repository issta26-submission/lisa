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
//<ID> 353
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
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);

    // step 2: Compress input with deflate to produce data for inflateBack
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)input;
    dstrm.avail_in = (uInt)inputLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong compLen = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflateBack, set undermine, and run inflateBack with callbacks
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    struct InDesc { const unsigned char * data; uLong size; uLong pos; };
    struct OutDesc { unsigned char * buf; uLong size; uLong pos; };
    InDesc indesc;
    indesc.data = compBuf;
    indesc.size = compLen;
    indesc.pos = 0;
    OutDesc outdesc;
    outdesc.buf = (unsigned char *)malloc((size_t)inputLen);
    memset(outdesc.buf, 0, (size_t)inputLen);
    outdesc.size = inputLen;
    outdesc.pos = 0;

    unsigned int (*in_cb)(void *, unsigned char **) = [](void * desc, unsigned char ** buf)->unsigned int {
        InDesc * d = (InDesc *)desc;
        unsigned int avail = (unsigned int)(d->size - d->pos);
        *buf = (unsigned char *)(d->data + d->pos);
        d->pos += avail;
        return avail;
    };
    int (*out_cb)(void *, unsigned char *, unsigned int) = [](void * desc, unsigned char * out, unsigned int len)->int {
        OutDesc * d = (OutDesc *)desc;
        memcpy(d->buf + d->pos, out, (size_t)len);
        d->pos += len;
        return 0;
    };

    inflateUndermine(&istrm, 1);
    inflateBack(&istrm, in_cb, (void *)&indesc, out_cb, (void *)&outdesc);
    inflateBackEnd(&istrm);

    // step 4: Write compressed data to gz file and Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose(gz);
    free(compBuf);
    free(window);
    free(outdesc.buf);
    return 66;
    // API sequence test completed successfully
}