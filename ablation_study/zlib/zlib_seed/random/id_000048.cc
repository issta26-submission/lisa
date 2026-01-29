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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char dict_c[] = "example zlib dictionary content";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char src_c[] = "payload data that will be referenced for bounds";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    z_stream infstrm;
    z_stream defstrm;
    memset(&infstrm, 0, sizeof(z_stream));
    memset(&defstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit2_(&infstrm, 15, version, (int)sizeof(z_stream));
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    int rc_def_setdict = deflateSetDictionary(&defstrm, dictionary, dictLen);
    int rc_inf_setdict = inflateSetDictionary(&infstrm, dictionary, dictLen);

    // step 3: Operate -> Validate
    uLong bound = deflateBound(&defstrm, sourceLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    off64_t offset_before = gzoffset64(gz);
    int rc_printf = gzprintf(gz, "ver=%s rc_def_init=%d rc_inf_init=%d rc_def_setdict=%d rc_inf_setdict=%d bound=%lu offset=%lld\n",
                             version, rc_def_init, rc_inf_init, rc_def_setdict, rc_inf_setdict, (unsigned long)bound, (long long)offset_before);

    // step 4: Cleanup
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inf_end = inflateEnd(&infstrm);
    int rc_close = gzclose(gz);
    (void)rc_inf_init; (void)rc_def_init; (void)rc_def_setdict; (void)rc_inf_setdict; (void)bound; (void)rc_printf; (void)rc_def_end; (void)rc_inf_end; (void)rc_close;

    // API sequence test completed successfully
    return 66;
}