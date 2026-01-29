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
//<ID> 36
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
    unsigned char input[128];
    unsigned char outbuf[256];
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    memset(&dstrm, 0, sizeof(dstrm));
    memset(input, 'A', sizeof(input));
    memset(outbuf, 0, sizeof(outbuf));
    uLong sourceLen = (uLong)sizeof(input);
    uLong compBound = compressBound(sourceLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(comp, &compLen, input, sourceLen, 6);
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)sizeof(outbuf);
    inflateCopy(&istrm_copy, &istrm);
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sourceLen;
    uLong defBound = deflateBound(&dstrm, sourceLen);
    unsigned char *defout = (unsigned char *)malloc((size_t)defBound);
    dstrm.next_out = defout;
    dstrm.avail_out = (uInt)defBound;

    // step 2: Configure
    deflateResetKeep(&dstrm);

    // step 3: Operate & Validate
    in_func in_cb = [](void * desc, unsigned char ** buf) -> unsigned int {
        z_stream *s = (z_stream *)desc;
        *buf = s->next_in;
        unsigned int n = s->avail_in;
        s->avail_in = 0;
        return n;
    };
    out_func out_cb = [](void * desc, unsigned char * buf, unsigned int len) -> int {
        z_stream *s = (z_stream *)desc;
        unsigned int copy = (len <= s->avail_out) ? len : s->avail_out;
        memcpy(s->next_out, buf, (size_t)copy);
        s->next_out += copy;
        s->avail_out -= copy;
        s->total_out += copy;
        return (int)copy;
    };
    istrm_copy.next_out = outbuf;
    istrm_copy.avail_out = (uInt)sizeof(outbuf);
    inflateBack(&istrm_copy, in_cb, &istrm_copy, out_cb, &istrm_copy);
    inflateBackEnd(&istrm_copy);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outbuf, (unsigned int)istrm_copy.total_out);
    gzflush(gz, 0);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);
    gzclose(gz);
    free(comp);
    free(defout);

    // API sequence test completed successfully
    return 66;
}