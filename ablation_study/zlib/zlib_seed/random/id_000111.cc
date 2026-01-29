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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&infstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    const char dict_c[] = "example_dictionary";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char src_c[] = "Hello zlib payload";
    const Bytef *src = (const Bytef *)src_c;
    uInt srcLen = (uInt)(sizeof(src_c) - 1);

    // step 2: Setup (initialize deflate stream and set dictionary; open gz file and write sample data)
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&defstrm, dictionary, dictLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, src, (unsigned int)srcLen);

    // step 3: Core operations (initialize inflate stream and validate it)
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int rc_inf_val = inflateValidate(&infstrm, 0);

    // step 4: Validate & Cleanup
    int rc_def_end = deflateEnd(&defstrm);
    int rc_gz_close = gzclose(gz);
    int rc_inf_end = inflateEnd(&infstrm);
    (void)rc_def_init; (void)rc_set_dict; (void)rc_gz_write;
    (void)rc_inf_init; (void)rc_inf_val; (void)rc_def_end;
    (void)rc_gz_close; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}