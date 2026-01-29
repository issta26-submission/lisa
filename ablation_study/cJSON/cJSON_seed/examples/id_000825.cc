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
//<ID> 825
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_pi = cJSON_CreateNumber(3.1415926535);
    cJSON_AddItemToObject(root, "pi", num_pi);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", null_item);
    cJSON *raw_item = cJSON_CreateRaw("{\"raw_flag\":true}");
    cJSON_AddItemToObject(root, "raw_blob", raw_item);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "answer", answer);
    cJSON *ver = cJSON_CreateString("1.0.0");
    cJSON_AddItemToObject(meta, "version", ver);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_answer = cJSON_GetObjectItem(parsed_meta, "answer");
    double parsed_value = cJSON_GetNumberValue(parsed_answer);
    cJSON_bool was_null = cJSON_IsNull(null_item);
    cJSON_bool was_raw = cJSON_IsRaw(raw_item);
    (void)parsed_value;
    (void)was_null;
    (void)was_raw;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}