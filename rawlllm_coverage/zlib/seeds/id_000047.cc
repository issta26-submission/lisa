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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function exercises a sequence of zlib APIs:
    // - compress / uncompress (simple API)
    // - deflateInit / deflate / deflateEnd (streaming compression)
    // - inflateInit / inflate / inflateEnd (streaming decompression)
    // - crc32 and adler32
    // - gzopen / gzwrite / gzread / gzclose (gz file API)
    //
    // Returns 66 on success, non-66 on failure.

    const char *sample = "The quick brown fox jumps over the lazy dog";
    // include the terminating NUL so round-trip string compare is simple
    unsigned long orig_len = (unsigned long)(strlen(sample) + 1);

    // 1) Simple compress / uncompress
    {
        unsigned long bound = compressBound(orig_len);
        unsigned char *comp = (unsigned char *)malloc(bound);
        if (!comp) return 1;
        unsigned long comp_size = (unsigned long)bound;
        int rc = compress(comp, &comp_size, (const unsigned char *)sample, orig_len);
        if (rc != Z_OK) { free(comp); return 2; }

        // uncompress
        unsigned char *uncomp = (unsigned char *)malloc(orig_len);
        if (!uncomp) { free(comp); return 3; }
        unsigned long uncomp_size = orig_len;
        rc = uncompress(uncomp, &uncomp_size, comp, comp_size);
        if (rc != Z_OK || uncomp_size != orig_len || memcmp(uncomp, sample, orig_len) != 0) {
            free(comp);
            free(uncomp);
            return 4;
        }

        free(comp);
        free(uncomp);
    }

    // 2) Streaming deflate (compress)
    unsigned char *stream_comp = NULL;
    size_t stream_comp_size = 0;
    {
        z_stream defstream;
        memset(&defstream, 0, sizeof(defstream));
        int rc = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
        if (rc != Z_OK) return 5;

        defstream.next_in = (unsigned char *)sample;
        defstream.avail_in = (uInt)orig_len;

        unsigned char outbuf[256];
        // First compress input with Z_NO_FLUSH until input consumed,
        // then finish with Z_FINISH.
        while (defstream.avail_in > 0) {
            defstream.next_out = outbuf;
            defstream.avail_out = (uInt)sizeof(outbuf);
            rc = deflate(&defstream, Z_NO_FLUSH);
            if (rc != Z_OK && rc != Z_STREAM_END) {
                deflateEnd(&defstream);
                free(stream_comp);
                return 6;
            }
            size_t have = sizeof(outbuf) - defstream.avail_out;
            if (have) {
                unsigned char *tmp = (unsigned char *)realloc(stream_comp, stream_comp_size + have);
                if (!tmp) {
                    deflateEnd(&defstream);
                    free(stream_comp);
                    return 7;
                }
                stream_comp = tmp;
                memcpy(stream_comp + stream_comp_size, outbuf, have);
                stream_comp_size += have;
            }
        }
        // Finish
        for (;;) {
            defstream.next_out = outbuf;
            defstream.avail_out = (uInt)sizeof(outbuf);
            rc = deflate(&defstream, Z_FINISH);
            if (rc != Z_OK && rc != Z_STREAM_END) {
                deflateEnd(&defstream);
                free(stream_comp);
                return 8;
            }
            size_t have = sizeof(outbuf) - defstream.avail_out;
            if (have) {
                unsigned char *tmp = (unsigned char *)realloc(stream_comp, stream_comp_size + have);
                if (!tmp) {
                    deflateEnd(&defstream);
                    free(stream_comp);
                    return 9;
                }
                stream_comp = tmp;
                memcpy(stream_comp + stream_comp_size, outbuf, have);
                stream_comp_size += have;
            }
            if (rc == Z_STREAM_END) break;
        }

        deflateEnd(&defstream);
        if (stream_comp_size == 0) { free(stream_comp); return 10; }
    }

    // 3) Streaming inflate (decompress the streaming-compressed data)
    unsigned char *stream_uncomp = NULL;
    size_t stream_uncomp_size = 0;
    {
        z_stream infstream;
        memset(&infstream, 0, sizeof(infstream));
        int rc = inflateInit(&infstream);
        if (rc != Z_OK) { free(stream_comp); return 11; }

        infstream.next_in = stream_comp;
        infstream.avail_in = (uInt)stream_comp_size;

        unsigned char outbuf[256];
        for (;;) {
            infstream.next_out = outbuf;
            infstream.avail_out = (uInt)sizeof(outbuf);
            rc = inflate(&infstream, Z_NO_FLUSH);
            if (rc != Z_OK && rc != Z_STREAM_END) {
                inflateEnd(&infstream);
                free(stream_comp);
                free(stream_uncomp);
                return 12;
            }
            size_t have = sizeof(outbuf) - infstream.avail_out;
            if (have) {
                unsigned char *tmp = (unsigned char *)realloc(stream_uncomp, stream_uncomp_size + have);
                if (!tmp) {
                    inflateEnd(&infstream);
                    free(stream_comp);
                    free(stream_uncomp);
                    return 13;
                }
                stream_uncomp = tmp;
                memcpy(stream_uncomp + stream_uncomp_size, outbuf, have);
                stream_uncomp_size += have;
            }
            if (rc == Z_STREAM_END) break;
            if (infstream.avail_in == 0 && rc == Z_OK && have == 0) {
                // no progress - error
                inflateEnd(&infstream);
                free(stream_comp);
                free(stream_uncomp);
                return 14;
            }
        }

        inflateEnd(&infstream);

        // Compare to original
        if (stream_uncomp_size != orig_len || memcmp(stream_uncomp, sample, orig_len) != 0) {
            free(stream_comp);
            free(stream_uncomp);
            return 15;
        }
    }

    // 4) crc32 and adler32 checksums
    {
        unsigned long crc_a = crc32(0L, Z_NULL, 0);
        crc_a = crc32(crc_a, (const unsigned char *)sample, orig_len);
        unsigned long crc_b = crc32(0L, Z_NULL, 0);
        crc_b = crc32(crc_b, stream_uncomp, (unsigned long)stream_uncomp_size);
        if (crc_a != crc_b) {
            free(stream_comp);
            free(stream_uncomp);
            return 16;
        }

        unsigned long ad_a = adler32(0L, Z_NULL, 0);
        ad_a = adler32(ad_a, (const unsigned char *)sample, orig_len);
        unsigned long ad_b = adler32(0L, Z_NULL, 0);
        ad_b = adler32(ad_b, stream_uncomp, (unsigned long)stream_uncomp_size);
        if (ad_a != ad_b) {
            free(stream_comp);
            free(stream_uncomp);
            return 17;
        }
    }

    // 5) gz write / read
    {
        const char *fname = "test_zlib_api_sequence_tmp.gz";
        gzFile gz = gzopen(fname, "wb");
        if (!gz) {
            free(stream_comp);
            free(stream_uncomp);
            return 18;
        }
        int written = gzwrite(gz, sample, (unsigned int)orig_len);
        if (written != (int)orig_len) {
            gzclose(gz);
            free(stream_comp);
            free(stream_uncomp);
            return 19;
        }
        gzclose(gz);

        gz = gzopen(fname, "rb");
        if (!gz) {
            free(stream_comp);
            free(stream_uncomp);
            return 20;
        }
        unsigned char *gzbuf = (unsigned char *)malloc(orig_len);
        if (!gzbuf) {
            gzclose(gz);
            free(stream_comp);
            free(stream_uncomp);
            return 21;
        }
        int read = gzread(gz, gzbuf, (unsigned int)orig_len);
        gzclose(gz);
        if (read != (int)orig_len || memcmp(gzbuf, sample, orig_len) != 0) {
            free(gzbuf);
            free(stream_comp);
            free(stream_uncomp);
            // attempt to remove file
            remove(fname);
            return 22;
        }
        free(gzbuf);
        remove(fname);
    }

    // 6) Additional check: uncompress streaming-compressed data using uncompress (simple API)
    {
        unsigned long outlen = orig_len;
        unsigned char *outbuf = (unsigned char *)malloc(outlen);
        if (!outbuf) {
            free(stream_comp);
            free(stream_uncomp);
            return 23;
        }
        int rc = uncompress(outbuf, &outlen, stream_comp, (unsigned long)stream_comp_size);
        if (rc != Z_OK || outlen != orig_len || memcmp(outbuf, sample, orig_len) != 0) {
            free(outbuf);
            free(stream_comp);
            free(stream_uncomp);
            return 24;
        }
        free(outbuf);
    }

    // Cleanup
    free(stream_comp);
    free(stream_uncomp);

    // All checks passed
    return 66;
}