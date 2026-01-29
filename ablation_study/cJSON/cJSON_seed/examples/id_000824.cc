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
//<ID> 824
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_answer);
    cJSON *raw_payload = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToObject(root, "payload", raw_payload);
    cJSON *missing_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", missing_item);

    // step 2: Configure
    cJSON_bool payload_is_raw = cJSON_IsRaw(raw_payload);
    cJSON *payload_raw_flag = cJSON_CreateNumber((double)payload_is_raw);
    cJSON_AddItemToObject(root, "payload_is_raw", payload_raw_flag);
    cJSON_bool missing_is_null = cJSON_IsNull(missing_item);
    cJSON *missing_flag = cJSON_CreateNumber((double)missing_is_null);
    cJSON_AddItemToObject(root, "missing_is_null", missing_flag);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_answer = cJSON_GetObjectItem(parsed, "answer");
    double parsed_value = cJSON_GetNumberValue(parsed_answer);
    cJSON *validated = cJSON_CreateNumber(parsed_value);
    cJSON_AddItemToObject(parsed, "validated_answer", validated);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}