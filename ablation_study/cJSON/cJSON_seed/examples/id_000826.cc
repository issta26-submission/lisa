#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 826
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", num);
    cJSON *raw = cJSON_CreateRaw("{\"inner\":true}");
    cJSON_AddItemToObject(root, "payload", raw);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", nul);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *index = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "index", index);
    cJSON *ref_raw = cJSON_CreateRaw("unparsed-raw");
    cJSON_AddItemToObject(meta, "ref", ref_raw);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_index = cJSON_GetObjectItem(parsed_meta, "index");
    double index_value = cJSON_GetNumberValue(parsed_index);
    cJSON *parsed_missing = cJSON_GetObjectItem(parsed, "missing");
    cJSON_bool missing_was_null = cJSON_IsNull(parsed_missing);
    cJSON *parsed_payload = cJSON_GetObjectItem(parsed, "payload");
    cJSON_bool payload_was_raw = cJSON_IsRaw(parsed_payload);
    char *reprint = cJSON_PrintUnformatted(parsed);
    (void)index_value;
    (void)missing_was_null;
    (void)payload_was_raw;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_free(reprint);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}