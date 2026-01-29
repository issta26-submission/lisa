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
//<ID> 882
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Example payload for zlib API sequence: compress, checksum, write.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong out_bound = deflateBound(&def, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_in = (Bytef *)source;
    def.avail_in = (uInt)source_len;
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    uLong checksum = adler32(0UL, source, (uInt)source_len);
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gz_cl = gzclose(gzf);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)checksum;
    (void)gz_wr;
    (void)gz_cl;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}