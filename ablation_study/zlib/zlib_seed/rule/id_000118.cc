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
//<ID> 118
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char input[128];
    unsigned char def_out[256];
    unsigned char infl_back_out[256];
    memset(input, 'Z', sizeof(input));
    memset(def_out, 0, sizeof(def_out));
    memset(infl_back_out, 0, sizeof(infl_back_out));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *comp_buf = (unsigned char *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);

    // step 2: Configure (deflate with extended init)
    deflateInit2_(&strm, 6, 8 /*Z_DEFLATED*/, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)inputLen;
    strm.next_out = (Bytef *)def_out;
    strm.avail_out = (uInt)sizeof(def_out);
    deflate(&strm, 0);
    unsigned int deflated_len = (unsigned int)strm.total_out;

    // step 3: Operate (use compress API and write compressed blob with gzputs)
    uLongf comp_len = (uLongf)bound;
    compress((Bytef *)comp_buf, &comp_len, (const Bytef *)input, inputLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)comp_buf);
    gzclose(gz);

    // step 4: Validate & Cleanup (call inflateBack and cleanup deflate state and buffers)
    strm.next_in = (Bytef *)comp_buf;
    strm.avail_in = (uInt)comp_len;
    strm.next_out = (Bytef *)infl_back_out;
    strm.avail_out = (uInt)sizeof(infl_back_out);
    inflateBack(&strm, (in_func)0, (void *)0, (out_func)0, (void *)0);
    deflateEnd(&strm);
    free(comp_buf);

    // API sequence test completed successfully
    return 66;
}