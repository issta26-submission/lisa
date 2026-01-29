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
//<ID> 1008
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: build compressed buffer, then use inflateCopy and inflateSetDictionary.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream defl;
    memset(&defl, 0, sizeof(defl));
    deflateInit_(&defl, 6, version, (int)sizeof(z_stream));
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    defl.next_in = (Bytef *)source;
    defl.avail_in = (uInt)source_len;
    defl.next_out = comp_buf;
    defl.avail_out = (uInt)bound;
    deflate(&defl, 0);
    uLong comp_size = defl.total_out;
    deflateEnd(&defl);

    // step 2: Configure
    z_stream inf_src;
    z_stream inf_copy;
    memset(&inf_src, 0, sizeof(inf_src));
    memset(&inf_copy, 0, sizeof(inf_copy));
    inflateInit_(&inf_src, version, (int)sizeof(z_stream));
    inf_src.next_in = comp_buf;
    inf_src.avail_in = (uInt)comp_size;
    Bytef *out_buf = (Bytef *)malloc((size_t)source_len);
    memset(out_buf, 0, (size_t)source_len);
    inf_src.next_out = out_buf;
    inf_src.avail_out = (uInt)source_len;
    inflateCopy(&inf_copy, &inf_src);
    inflateSetDictionary(&inf_copy, source, (uInt)source_len);

    // step 3: Operate
    const char *fname = "zlib_test_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gz_written = gzwrite(gzf_w, comp_buf, (unsigned int)comp_size);
    gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    int first_byte = gzgetc(gzf_r);
    gzclose(gzf_r);

    // step 4: Validate / Cleanup
    inflateEnd(&inf_src);
    inflateEnd(&inf_copy);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    (void)gz_written;
    (void)first_byte;
    return 66;
    // API sequence test completed successfully
}