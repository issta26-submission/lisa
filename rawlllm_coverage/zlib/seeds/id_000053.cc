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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a sequence of zlib API calls:
    // - compress() / uncompress()
    // - deflateInit() / deflate() / deflateEnd()
    // - inflateInit() / inflate() / inflateEnd()
    // If all checks pass the function returns 66.
    //
    // Note: This function expects that the translation unit including it
    // already includes the zlib header (<zlib.h>) and the standard C
    // runtime headers (for malloc / free / strlen / memcmp). If needed,
    // add the appropriate includes in your file.

    const char *src = "The quick brown fox jumps over the lazy dog";
    // src_len is small so casting to zlib's integer types below is safe
    uLong src_len = static_cast<uLong>(strlen(src));

    // 1) High-level compress() / uncompress()
    uLong bound = compressBound(src_len);
    Bytef *comp = static_cast<Bytef*>(malloc(bound));
    if (!comp) return -1;

    uLongf comp_size = static_cast<uLongf>(bound);
    int ret = compress(comp, &comp_size, reinterpret_cast<const Bytef*>(src), src_len);
    if (ret != Z_OK) { free(comp); return -2; }

    Bytef *uncmp = static_cast<Bytef*>(malloc(src_len + 1)); // +1 for safety / NUL
    if (!uncmp) { free(comp); return -3; }
    uLongf uncmp_size = static_cast<uLongf>(src_len);
    ret = uncompress(uncmp, &uncmp_size, comp, comp_size);
    if (ret != Z_OK || uncmp_size != src_len) { free(comp); free(uncmp); return -4; }
    uncmp[src_len] = 0;
    if (memcmp(uncmp, src, src_len) != 0) { free(comp); free(uncmp); return -5; }
    free(uncmp);

    // 2) Lower-level deflate (one-shot) producing zlib-wrapped data
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) { free(comp); return -6; }

    dstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src));
    dstrm.avail_in = static_cast<uInt>(src_len);

    uLongf def_bound = compressBound(src_len);
    Bytef *def_comp = static_cast<Bytef*>(malloc(def_bound));
    if (!def_comp) { deflateEnd(&dstrm); free(comp); return -7; }

    dstrm.next_out = def_comp;
    dstrm.avail_out = static_cast<uInt>(def_bound);

    ret = deflate(&dstrm, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&dstrm); free(comp); free(def_comp); return -8; }
    uLongf def_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // 3) Inflate the deflated data and verify it matches the original
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    ret = inflateInit(&istrm);
    if (ret != Z_OK) { free(comp); free(def_comp); return -9; }

    istrm.next_in = def_comp;
    istrm.avail_in = static_cast<uInt>(def_size);

    Bytef *out = static_cast<Bytef*>(malloc(src_len + 1));
    if (!out) { inflateEnd(&istrm); free(comp); free(def_comp); return -10; }

    istrm.next_out = out;
    istrm.avail_out = static_cast<uInt>(src_len);

    ret = inflate(&istrm, Z_FINISH);
    if (ret != Z_STREAM_END) { inflateEnd(&istrm); free(comp); free(def_comp); free(out); return -11; }
    uLongf out_size = istrm.total_out;
    inflateEnd(&istrm);

    if (out_size != src_len) { free(comp); free(def_comp); free(out); return -12; }
    if (memcmp(out, src, src_len) != 0) { free(comp); free(def_comp); free(out); return -13; }

    // Clean up
    free(out);
    free(def_comp);
    free(comp);

    // Optional: touch a couple of informational APIs to show usage
    const char *version = zlibVersion();
    unsigned long flags = zlibCompileFlags();
    (void)version;
    (void)flags;

    // All checks passed
    return 66;
}