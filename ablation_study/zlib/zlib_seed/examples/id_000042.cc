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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char dict_c[] = "example_dictionary_for_zlib_usage";
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    Bytef *dict = (Bytef *)malloc((size_t)dictLen);
    memcpy(dict, dict_c, (size_t)dictLen);
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate and inflate)
    int rc_deflateInit = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflateInit = inflateInit2_(&istrm, 15, version, (int)sizeof(z_stream));

    // step 3: Operate -> Validate (set dictionaries, open gzip, report status)
    int rc_def_set = deflateSetDictionary(&dstrm, dict, dictLen);
    int rc_inf_set = inflateSetDictionary(&istrm, dict, dictLen);
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    off64_t offset = gzoffset64(gz);
    int rc_gzprintf = gzprintf(gz, "ver=%s def_init=%d inf_init=%d def_set=%d inf_set=%d off=%lld\n",
                               version, rc_deflateInit, rc_inflateInit, rc_def_set, rc_inf_set, (long long)offset);

    // step 4: Cleanup
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gz);
    free(dict);
    (void)rc_deflateInit; (void)rc_inflateInit; (void)rc_def_set; (void)rc_inf_set;
    (void)rc_gzprintf; (void)rc_def_end; (void)rc_inf_end; (void)rc_gzclose; (void)version; (void)offset;

    // API sequence test completed successfully
    return 66;
}