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
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

/* Helper state for the in_func used by inflateBack.
   We return the entire compressed buffer size once, then 0. */
static unsigned g_in_len_for_infunc = 0;
static unsigned my_in_func(void *in_desc, unsigned char **buf) {
    *buf = (unsigned char *)in_desc;
    unsigned ret = g_in_len_for_infunc;
    g_in_len_for_infunc = 0;
    return ret;
}

static int my_out_func(void *out_desc, unsigned char *buf, unsigned int len) {
    /* Copy produced output into the provided out_desc buffer.
       We ignore overlap/advancement for this straight-line test. */
    memcpy(out_desc, buf, len);
    return 0; /* return 0 to indicate success to inflateBack */
}

int test_zlib_api_sequence() {
    /* step 1: Declarations & Initialize */
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[128];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    const char * version = zlibVersion();
    deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    /* step 2: Configure (prepare compression buffers and dictionary) */
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;

    /* step 3: Operate (compress, reset-keep, set inflate dictionary, compute checksum, use inflateBack and inflate) */
    deflate(&def_strm, 4); /* 4 corresponds to Z_FINISH in zlib */
    deflateResetKeep(&def_strm);
    uLong checksum = adler32(0UL, (const Bytef *)input, (uInt)inputLen);
    (void)checksum;
    inflateSetDictionary(&inf_strm, (const Bytef *)input, (uInt)8);

    /* Prepare callbacks and pass buffers to inflateBack */
    in_func in_cb = (in_func)my_in_func;
    out_func out_cb = (out_func)my_out_func;
    /* Provide the compressed length to the in-callback helper */
    g_in_len_for_infunc = (unsigned)def_strm.total_out;

    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    unsigned char outBuf[256];
    memset(outBuf, 0, sizeof(outBuf));
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)sizeof(outBuf);

    inflateBack(&inf_strm, in_cb, (void *)compBuf, out_cb, (void *)outBuf);
    inflate(&inf_strm, 0);

    /* step 4: Validate & Cleanup */
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    free(compBuf);

    return 66;
}