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
//<ID> 628
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    int numbers[] = { 10, 20, 30 };
    cJSON *num_array = cJSON_CreateIntArray(numbers, 3);
    cJSON *null_item = cJSON_CreateNull();
    char *tmp_buf = (char *)cJSON_malloc(256);
    memset(tmp_buf, 0, 256);

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", num_array);
    cJSON_AddItemToObject(root, "nullable", null_item);
    cJSON_AddNumberToObject(root, "constant", 7.0);
    cJSON_AddStringToObject(root, "label", "cjson_test");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *first_num_item = cJSON_GetArrayItem(parsed_numbers, 0);
    int first_value = (int)cJSON_GetNumberValue(first_num_item);
    cJSON_bool parsed_is_null = cJSON_IsNull(cJSON_GetObjectItem(parsed, "nullable"));
    cJSON_DeleteItemFromObject(parsed, "label");
    sprintf(tmp_buf, "ver=%s;first=%d;is_null=%d",
            version ? version : "null",
            first_value,
            (int)parsed_is_null);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(tmp_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}