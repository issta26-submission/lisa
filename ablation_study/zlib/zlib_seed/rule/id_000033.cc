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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper input function for inflateBack: supplies compressed bytes from a buffer */
typedef struct { unsigned char *data; uLong len; } in_state;
static unsigned in_from_buffer(void *desc, unsigned char **buf) {
    in_state *s = (in_state *)desc;
    if (!s->len) return 0;
    *buf = s->data;
    unsigned ret = (unsigned)s->len;
    s->data += ret;
    s->len = 0;
    return ret;
}

/* Helper output function for inflateBack: writes decompressed bytes into a gzFile */
static int out_to_gz(void *desc, unsigned char *buf, unsigned int len) {
    return gzwrite((gzFile)desc, buf, len);
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & Initialize */
    z_stream dstrm;
    z_stream istrm;
    z_stream istrm_copy;
    unsigned char input[256];
    unsigned char *comp = NULL;
    unsigned char *decomp = NULL;
    gzFile gz = 0;
    unsigned char *window = NULL;
    in_state istate;

    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    memset(input, 'X', sizeof(input));

    uLong sourceLen = (uLong)sizeof(input);
    uLong cb = compressBound(sourceLen);
    comp = (unsigned char *)malloc((size_t)cb);
    decomp = (unsigned char *)malloc((size_t)sourceLen);
    memset(comp, 0, (size_t)cb);
    memset(decomp, 0, (size_t)sourceLen);

    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    /* allocate a window buffer for inflateBackInit_ */
    window = (unsigned char *)malloc(1 << 15);
    inflateBackInit_(&istrm, 15, window, zlibVersion(), (int)sizeof(z_stream));

    /* step 2: Configure */
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)cb;
    deflate(&dstrm, Z_FINISH);

    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)sourceLen;
    inflateCopy(&istrm_copy, &istrm);

    deflateResetKeep(&dstrm);

    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, comp, (unsigned int)dstrm.total_out);
    gzflush(gz, 0);

    /* prepare state for inflateBack */
    istate.data = comp;
    istate.len = dstrm.total_out;

    /* step 3: Operate & Validate */
    inflateBack(&istrm_copy, in_from_buffer, &istate, out_to_gz, (void *)gz);
    inflateBackEnd(&istrm_copy);

    /* step 4: Cleanup */
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);
    gzclose(gz);
    free(comp);
    free(decomp);
    free(window);

    return 66;
}