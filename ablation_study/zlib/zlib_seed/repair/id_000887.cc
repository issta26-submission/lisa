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
//<ID> 887
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef input[] = "Example payload for zlib compression and gz write test.";
    uLong input_len = (uLong)(sizeof(input) - 1);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    strm.next_in = (Bytef *)input;
    strm.avail_in = (uInt)input_len;
    uLong out_bound = deflateBound(&strm, input_len);
    Bytef *outbuf = (Bytef *)malloc((size_t)out_bound);
    memset(outbuf, 0, (size_t)out_bound);
    strm.next_out = outbuf;
    strm.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int gz_wr = gzwrite(gzf, outbuf, (unsigned int)comp_len);
    int gz_cl = gzclose(gzf);
    uLong checksum = adler32(0UL, input, (uInt)input_len);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&strm);
    free(outbuf);
    (void)version;
    (void)init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)gz_wr;
    (void)gz_cl;
    (void)checksum;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}