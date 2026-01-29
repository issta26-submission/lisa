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
//<ID> 1002
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test: produce compressed data, copy inflate state, set dictionary, and use gz APIs.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    (void)def_init_ret;
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)source_len;
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&defstrm, 0);
    uLong comp_size = defstrm.total_out;
    int def_end_ret = deflateEnd(&defstrm);
    (void)def_ret;
    (void)def_end_ret;

    // step 2: Configure
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    (void)inf_init_ret;
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_size;
    z_stream copystrm;
    memset(&copystrm, 0, sizeof(copystrm));
    int copy_ret = inflateCopy(&copystrm, &infstrm);
    (void)copy_ret;

    // step 3: Operate
    uInt dictLength = (uInt)source_len;
    Bytef *dictBuf = (Bytef *)malloc((size_t)dictLength);
    memset(dictBuf, 0, (size_t)dictLength);
    int setdict_ret = inflateSetDictionary(&copystrm, dictBuf, dictLength);
    (void)setdict_ret;
    Bytef *out_buf = (Bytef *)malloc((size_t)(source_len + 16));
    memset(out_buf, 0, (size_t)(source_len + 16));
    copystrm.next_out = out_buf;
    copystrm.avail_out = (uInt)(source_len + 16);
    int inf_ret = inflate(&copystrm, 0);
    (void)inf_ret;
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzwr = gzwrite(gz_w, comp_buf, (unsigned int)comp_size);
    int gzclose_w = gzclose(gz_w);
    (void)gzwr;
    (void)gzclose_w;
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_char = gzgetc(gz_r);
    int gzclose_r = gzclose(gz_r);
    (void)first_char;
    (void)gzclose_r;

    // step 4: Validate / Cleanup
    int inflate_copy_end = inflateEnd(&copystrm);
    int inflate_orig_end = inflateEnd(&infstrm);
    free(comp_buf);
    free(dictBuf);
    free(out_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    (void)inflate_copy_end;
    (void)inflate_orig_end;
    // API sequence test completed successfully
    return 66;
}