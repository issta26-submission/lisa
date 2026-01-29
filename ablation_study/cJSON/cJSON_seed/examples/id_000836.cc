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
//<ID> 836
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[] = {10.0, 20.0, 30.0};
    cJSON *values = cJSON_CreateDoubleArray(nums, 3);
    cJSON *raw_entry = cJSON_CreateRaw("{\"note\":\"raw\"}");
    cJSON_AddItemToObjectCS(root, "values", values);
    cJSON_AddItemToObjectCS(root, "raw", raw_entry);

    // step 2: Configure
    cJSON *new_number = cJSON_CreateNumber(99.9);
    cJSON_ReplaceItemInArray(values, 1, new_number);
    cJSON *missing = cJSON_CreateNull();
    cJSON_AddItemToObjectCS(root, "missing", missing);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItemCaseSensitive(parsed, "values");
    cJSON *second_item = cJSON_GetArrayItem(parsed_values, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    (void)second_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}