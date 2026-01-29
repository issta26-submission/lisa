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
//<ID> 853
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Sample data for zlib gzip seek/offset and inflate get dictionary";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    uLong bound = compressBound(sample_len);
    uLongf comp_len = 0;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    int comp_ret = compress2(comp_buf, &comp_len, sample, sample_len, 6);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 2: Configure
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf = gzopen(fname, "wb");
    int wrote = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off_t offset_after_write = gzoffset(gzf);
    off_t seek_ret = gzseek(gzf, 0, 0);
    off_t offset_after_seek = gzoffset(gzf);

    // step 3: Operate
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(sample_len + 64));
    memset(decomp_buf, 0, (size_t)(sample_len + 64));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(sample_len + 64);
    int prime_ret = inflatePrime(&inf, 1, 0);
    int inf_ret = inflate(&inf, 4);
    Bytef dictbuf[64];
    uInt dict_len = (uInt)sizeof(dictbuf);
    int dict_ret = inflateGetDictionary(&inf, dictbuf, &dict_len);

    // step 4: Validate / Cleanup
    int gz_close_ret = gzclose(gzf);
    int inf_end = inflateEnd(&inf);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)sample_len;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)inf_init;
    (void)wrote;
    (void)offset_after_write;
    (void)seek_ret;
    (void)offset_after_seek;
    (void)prime_ret;
    (void)inf_ret;
    (void)dict_ret;
    (void)dict_len;
    (void)gz_close_ret;
    (void)inf_end;
    (void)fname;
    // API sequence test completed successfully
    return 66;
}