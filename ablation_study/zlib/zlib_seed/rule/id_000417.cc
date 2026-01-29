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
//<ID> 417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

typedef struct {
    unsigned char *buf;
    unsigned int len;
} InDesc;

static unsigned int test_in(void *in_desc, unsigned char **buf) {
    InDesc *d = (InDesc *)in_desc;
    *buf = d->buf;
    unsigned int l = d->len;
    d->buf += l;
    d->len = 0;
    return l;
}

static int test_out(void *out_desc, unsigned char *buf, unsigned int len) {
    unsigned char *dest = (unsigned char *)out_desc;
    if (dest && buf && len) {
        memcpy(dest, buf, len);
    }
    return 0;
}

int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defStrm;
    memset(&defStrm, 0, sizeof(defStrm));
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    z_stream infCopy;
    memset(&infCopy, 0, sizeof(infCopy));
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    deflateInit_(&defStrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defStrm.next_in = (Bytef *)input;
    defStrm.avail_in = (uInt)inputLen;
    defStrm.next_out = compBuf;
    defStrm.avail_out = (uInt)bound;
    defStrm.adler = checksum;

    // step 2: Operate (deflate) and prepare inflate stream
    deflate(&defStrm, 0);
    inflateInit_(&infStrm, zlibVersion(), (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)defStrm.total_out;
    unsigned char outBuf[256];
    memset(outBuf, 0, sizeof(outBuf));
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;

    // step 3: Copy inflate state, get header, and call inflateBack
    inflateCopy(&infCopy, &infStrm);
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&infCopy, &head);
    InDesc inDesc;
    inDesc.buf = compBuf;
    inDesc.len = (unsigned int)infCopy.avail_in;
    inflateBack(&infCopy, (in_func)test_in, &inDesc, (out_func)test_out, outBuf);

    // step 4: Cleanup and finalize
    deflateEnd(&defStrm);
    inflateEnd(&infStrm);
    inflateEnd(&infCopy);
    free(compBuf);
    return 66;
    // API sequence test completed successfully
}