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
//<ID> 116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "mixed", arr);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, null_item);
    cJSON *raw_item = cJSON_CreateRaw("{\"inner\":null}");
    cJSON_AddItemToArray(arr, raw_item);
    cJSON *str_item = cJSON_CreateString("text");
    cJSON_AddItemToArray(arr, str_item);

    // step 2: Configure
    size_t buf_size = 64;
    char *json_buf = (char *)cJSON_malloc(buf_size);
    memset(json_buf, 0, buf_size);
    strcpy(json_buf, "  { \"parsed\": null, \"value\": \"ok\" }  ");
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *parsed_value = cJSON_GetObjectItem(parsed, "value");
    const char *parsed_text = cJSON_GetStringValue(parsed_value);
    cJSON *parsed_label = cJSON_CreateString(parsed_text);
    cJSON_AddItemToObject(root, "parsed_label", parsed_label);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_null = cJSON_IsNull(first);
    cJSON *isnull_num = cJSON_CreateNumber((double)first_is_null);
    cJSON_AddItemToObject(root, "first_is_null", isnull_num);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(json_buf);
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}