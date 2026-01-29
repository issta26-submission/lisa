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
//<ID> 354
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize (create raw DEFLATE stream)
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit2_(&dstrm, 6, 8, -15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
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

    // step 2: Configure inflateBack (initialize back-inflater with raw window)
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    memset(&istrm, 0, sizeof(istrm));
    inflateBackInit_(&istrm, -15, window, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate (inflateBack) and Validate (inflateUndermine)
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)inputLen;
    istrm.total_out = 0;
    in_func inf = static_cast<in_func>([](void * desc, unsigned char ** buf)->unsigned int {
        z_stream * s = (z_stream *)desc;
        unsigned int avail = s->avail_in;
        *buf = s->next_in;
        s->next_in += avail;
        s->avail_in = 0;
        return avail;
    });
    out_func outf = static_cast<out_func>([](void * desc, unsigned char * buf, unsigned int len)->int {
        z_stream * s = (z_stream *)desc;
        unsigned int copy = (s->avail_out < len) ? s->avail_out : len;
        memcpy(s->next_out, buf, copy);
        s->next_out += copy;
        s->avail_out -= copy;
        s->total_out += copy;
        return 0;
    });
    inflateBack(&istrm, inf, (void *)&istrm, outf, (void *)&istrm);
    inflateUndermine(&istrm, 1);
    inflateBackEnd(&istrm);

    // step 4: Write compressed data to gz file & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose(gz);
    free(compBuf);
    free(window);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}