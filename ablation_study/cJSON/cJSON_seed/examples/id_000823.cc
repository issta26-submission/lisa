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
//<ID> 823
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToObject(root, "payload", raw_item);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *factor = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(meta, "factor", factor);

    // step 3: Operate & Validate
    cJSON_bool null_check = cJSON_IsNull(maybe_null);
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_answer = cJSON_GetObjectItem(parsed, "answer");
    double parsed_number = cJSON_GetNumberValue(parsed_answer);
    (void)null_check;
    (void)raw_check;
    (void)parsed_number;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}