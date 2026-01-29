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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    memset(&inf_strm, 0, sizeof(z_stream));
    const char dict_c[] = "example_dictionary_for_zlib";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dict_len = (uInt)(sizeof(dict_c) - 1);
    const char src_c[] = "Sample payload data to be compressed and described by zlib sequence";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate & inflate and set dictionaries)
    int rc_def_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int rc_def_set = deflateSetDictionary(&def_strm, dictionary, dict_len);
    int rc_inf_init = inflateInit2_(&inf_strm, 15, version, (int)sizeof(z_stream));
    int rc_inf_set = inflateSetDictionary(&inf_strm, dictionary, dict_len);

    // step 3: Operate -> Validate (compress payload, write status to gzip and query offset)
    uLong bound = compressBound(sourceLen);
    Bytef *dest = (Bytef *)malloc((size_t)bound);
    uLongf destLen = (uLongf)bound;
    int rc_compress = compress(dest, &destLen, source, sourceLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_printf1 = gzprintf(gz, "ver=%s def_init=%d def_set=%d inf_init=%d inf_set=%d comp=%d compSize=%lu\n",
                              version, rc_def_init, rc_def_set, rc_inf_init, rc_inf_set, rc_compress, (unsigned long)destLen);
    off64_t offset = gzoffset64(gz);
    int rc_printf2 = gzprintf(gz, "gz_offset=%lld\n", (long long)offset);

    // step 4: Cleanup
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    gzclose(gz);
    free(dest);

    // API sequence test completed successfully
    (void)rc_def_init; (void)rc_def_set; (void)rc_inf_init; (void)rc_inf_set;
    (void)rc_compress; (void)rc_printf1; (void)rc_printf2; (void)offset;
    return 66;
}