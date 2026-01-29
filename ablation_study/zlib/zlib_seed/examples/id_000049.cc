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
//<ID> 49
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char dict_c[] = "example dictionary for zlib";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char src_c[] = "payload data to compress using zlib with dictionary";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    memset(&inf_strm, 0, sizeof(z_stream));
    uLong bound = compressBound(sourceLen);
    uLongf compBufLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    Bytef *decompBuf = (Bytef *)malloc((size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)sourceLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)bound;

    // step 2: Setup (initialize deflate, set dictionary)
    const char *version = zlibVersion();
    int rc_def_init = deflateInit2_(&def_strm, 6, 8, 15, 8, 0, version, (int)sizeof(z_stream));
    int rc_def_setdict = deflateSetDictionary(&def_strm, dictionary, dictLen);

    // step 3: Operate -> Compress, initialize inflate, set dictionary, decompress
    int rc_def_fin = deflate(&def_strm, 4);
    uLong comp_written = def_strm.total_out;
    int rc_def_end = deflateEnd(&def_strm);
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)comp_written;
    inf_strm.next_out = decompBuf;
    inf_strm.avail_out = (uInt)bound;
    int rc_inf_init = inflateInit2_(&inf_strm, 15, version, (int)sizeof(z_stream));
    int rc_inf_setdict = inflateSetDictionary(&inf_strm, dictionary, dictLen);
    int rc_inf = inflate(&inf_strm, 0);
    uLong decomp_written = inf_strm.total_out;

    // step 4: Validate & Cleanup (write results to gzip and release resources)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int wrote = gzwrite(gz, decompBuf, (unsigned int)decomp_written);
    off64_t offset = gzoffset64(gz);
    int rc_printf = gzprintf(gz, "ver=%s rc_def_init=%d rc_def_setdict=%d rc_def_fin=%d comp_written=%lu rc_inf_init=%d rc_inf_setdict=%d rc_inf=%d decomp_written=%lu offset=%lld\n",
                             version, rc_def_init, rc_def_setdict, rc_def_fin, (unsigned long)comp_written, rc_inf_init, rc_inf_setdict, rc_inf, (unsigned long)decomp_written, (long long)offset);
    inflateEnd(&inf_strm);
    gzclose(gz);
    free(compBuf);
    free(decompBuf);

    // API sequence test completed successfully
    (void)wrote; (void)rc_printf; (void)rc_def_end;
    return 66;
}