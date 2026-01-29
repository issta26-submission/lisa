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
//<ID> 1424
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload using deflateInit_/deflateCopy/deflateGetDictionary/gzopen/gzwrite";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup - initialize deflate stream and supply a preset dictionary
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    const char preset_dict[] = "preset_dictionary_data";
    uInt preset_dict_len = (uInt)(sizeof(preset_dict) - 1);
    int rc_set_dict = deflateSetDictionary(&dstrm, (const Bytef *)preset_dict, preset_dict_len);
    uLong bound = deflateBound(&dstrm, srcLen);
    Bytef *defBuf = new Bytef[(size_t)bound];
    memset(defBuf, 0, (size_t)bound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = defBuf;
    dstrm.avail_out = (uInt)bound;

    // step 3: Core operations - copy the deflate stream, perform compression, then retrieve the dictionary
    z_stream dcopy;
    memset(&dcopy, 0, (size_t)sizeof(z_stream));
    int rc_deflate_copy = deflateCopy(&dcopy, &dstrm);
    int rc_deflate = deflate(&dstrm, 0);
    uLong compLen = dstrm.total_out;
    Bytef dictOut[128];
    uInt dictOutLen = (uInt)sizeof(dictOut);
    int rc_get_dict = deflateGetDictionary(&dstrm, dictOut, &dictOutLen);

    // step 4: Finalize and cleanup - write original data to a gzip file, close and free resources
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    int rc_gzclose_w = gzclose_w(gzf);
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_deflate_end_copy = deflateEnd(&dcopy);

    delete [] src;
    delete [] defBuf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_set_dict;
    (void)bound;
    (void)rc_deflate_copy;
    (void)rc_deflate;
    (void)compLen;
    (void)rc_get_dict;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_deflate_end;
    (void)rc_deflate_end_copy;
    // API sequence test completed successfully
    return 66;
}