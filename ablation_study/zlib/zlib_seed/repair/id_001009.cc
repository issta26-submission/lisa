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
//<ID> 1009
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence sample data for inflate and gz operations.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    deflate(&def_strm, 0);
    uLong comp_size = def_strm.total_out;
    deflateEnd(&def_strm);

    // step 2: Configure
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    uLong out_len = source_len + 16;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_len);
    memset(out_buf, 0, (size_t)out_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_len;

    // step 3: Operate
    inflate(&inf_strm, 0);
    z_stream copy_strm;
    memset(&copy_strm, 0, sizeof(copy_strm));
    inflateCopy(&copy_strm, &inf_strm);
    uInt dictLen = (uInt)3;
    Bytef dictBuf[3];
    dictBuf[0] = source[0];
    dictBuf[1] = source[1];
    dictBuf[2] = source[2];
    inflateSetDictionary(&copy_strm, dictBuf, dictLen);
    inflateEnd(&copy_strm);
    gzFile out_gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(out_gz, comp_buf, (unsigned int)comp_size);
    gzclose(out_gz);
    gzFile in_gz = gzopen("tmp_zlib_api_seq.gz", "rb");
    int first_char = gzgetc(in_gz);
    gzclose(in_gz);

    // step 4: Validate / Cleanup
    inflateEnd(&inf_strm);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    (void)out_len;
    (void)first_char;
    // API sequence test completed successfully
    return 66;
}