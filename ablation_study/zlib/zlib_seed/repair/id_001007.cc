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
//<ID> 1007
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: prepare a stream, copy it, set a dictionary, and interact with gz file.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream src_strm;
    memset(&src_strm, 0, sizeof(src_strm));
    int init_ret = inflateInit_(&src_strm, version, (int)sizeof(z_stream));
    (void)init_ret;

    // step 2: Configure
    src_strm.next_in = (Bytef *)source;
    src_strm.avail_in = (uInt)source_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)source_len + 16);
    memset(out_buf, 0, (size_t)source_len + 16);
    src_strm.next_out = out_buf;
    src_strm.avail_out = (uInt)(source_len + 16);

    Bytef dict[] = "example-dictionary-for-inflate";
    uInt dict_len = (uInt)(sizeof(dict) - 1);

    // step 3: Operate
    z_stream dest_strm;
    memset(&dest_strm, 0, sizeof(dest_strm));
    int copy_ret = inflateCopy(&dest_strm, &src_strm);
    (void)copy_ret;
    int setdict_ret = inflateSetDictionary(&dest_strm, dict, dict_len);
    (void)setdict_ret;
    gzFile gzf = gzopen("test_zlib_api_sequence.tmp", "rb");
    int gzch = gzgetc(gzf);
    (void)gzch;

    // step 4: Validate / Cleanup
    int end_ret_src = inflateEnd(&src_strm);
    int end_ret_dest = inflateEnd(&dest_strm);
    (void)end_ret_src;
    (void)end_ret_dest;
    int gzclose_ret = gzclose(gzf);
    (void)gzclose_ret;
    free(out_buf);
    (void)version;
    (void)source_len;
    (void)dict_len;
    return 66;
    // API sequence test completed successfully
}