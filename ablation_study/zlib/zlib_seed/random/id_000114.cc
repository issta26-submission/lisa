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
//<ID> 114
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
    const char src_c[] = "Example payload for deflate with dictionary then write gz and validate inflate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char dict_c[] = "preset_dictionary";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);

    // step 2: Configure (initialize deflate and set dictionary)
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&defstrm, dictionary, dictLen);

    // step 3: Operate (compress input into buffer and write to gz file)
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)bound;
    int rc_def = deflate(&defstrm, 0);
    uLong produced = defstrm.total_out;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)produced);

    // step 4: Validate & Cleanup (initialize inflate, validate, and free resources)
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int rc_inf_valid = inflateValidate(&infstrm, 15);
    int rc_gz_close = gzclose(gz);
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inf_end = inflateEnd(&infstrm);
    free(compBuf);
    (void)rc_def_init; (void)rc_set_dict; (void)rc_def; (void)rc_gz_write;
    (void)rc_inf_init; (void)rc_inf_valid; (void)rc_gz_close; (void)rc_def_end; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}