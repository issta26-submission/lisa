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
//<ID> 1005
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test: compress, copy inflate state, set dictionary, and exercise gz I/O.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (compress)
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate (deflate and write compressed data to a gzip file)
    deflate(&def_strm, 0);
    uLong comp_size = def_strm.total_out;
    deflateEnd(&def_strm);
    gzFile gz_w = gzopen("tmp_zlib_api.gz", "wb");
    gzwrite(gz_w, comp_buf, (unsigned int)comp_size);
    gzclose(gz_w);

    // step 4: Validate / Cleanup (inflate init, copy, set dictionary, read via gzgetc, cleanup)
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    inflateCopy(&inf_copy, &inf_strm);
    Bytef dict[] = "preset_dictionary_for_inflate";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    inflateSetDictionary(&inf_copy, dict, dict_len);
    gzFile gz_r = gzopen("tmp_zlib_api.gz", "rb");
    int first_byte = gzgetc(gz_r);
    (void)first_byte;
    gzclose(gz_r);
    inflateEnd(&inf_copy);
    inflateEnd(&inf_strm);
    free(comp_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}