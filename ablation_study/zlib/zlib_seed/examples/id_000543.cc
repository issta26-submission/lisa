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
//<ID> 543
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, query zlib version, and initialize deflate stream
    const char src[] = "zlib API sequence payload: dictionary -> deflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure deflate with a dictionary, allocate output buffer, and perform deflate
    Bytef dict[] = { 'e','x','a','m','p','l','e','_','d','i','c','t' };
    uInt dictLen = (uInt)(sizeof(dict));
    int rc_set_dict = deflateSetDictionary(&dstrm, dict, dictLen);
    uLong dbound = deflateBound(&dstrm, sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)dbound);
    memset(comp, 0, (size_t)dbound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)dbound;
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */

    // step 3: Open a gzFile, write formatted data and a character, then close the file
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, (const char *)source);
    int rc_gzputc = gzputc(gf, (int)src[0]);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup deflate stream, free buffers and finalize
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    (void)version;
    (void)rc_def_init;
    (void)rc_set_dict;
    (void)rc_deflate;
    (void)rc_gzprintf;
    (void)rc_gzputc;
    (void)rc_gzclose;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}