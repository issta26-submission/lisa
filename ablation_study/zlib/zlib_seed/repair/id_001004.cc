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
//<ID> 1004
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: compress then inflate, set dictionary, copy inflater, and read gz file.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    (void)def_init_ret;

    // step 2: Configure (compress)
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 0);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    (void)def_ret;
    (void)def_end_ret;

    // step 3: Operate (inflate, set dictionary, copy inflater, gz open/getc)
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    (void)inf_init_ret;
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    int inf_ret = inflate(&inf_strm, 0);
    (void)inf_ret;
    uInt dictLength = (uInt)source_len;
    int dict_ret = inflateSetDictionary(&inf_strm, (const Bytef *)source, dictLength);
    (void)dict_ret;
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf_strm);
    (void)copy_ret;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "rb");
    int c = gzgetc(gzf);
    (void)c;

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int inf_copy_end_ret = inflateEnd(&inf_copy);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    (void)inf_end_ret;
    (void)inf_copy_end_ret;
    (void)gz_close_ret;
    return 66;
    // API sequence test completed successfully
}