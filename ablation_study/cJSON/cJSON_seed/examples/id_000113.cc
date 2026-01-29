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
//<ID> 113
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, nul);
    cJSON *raw = cJSON_CreateRaw("  { \"inner\": true }  ");
    cJSON_AddItemToArray(arr, raw);

    // step 2: Configure
    size_t buf_size = 64;
    char *buf = (char *)cJSON_malloc(buf_size);
    memset(buf, 0, buf_size);
    strcpy(buf, "  { \"min\" : \"  spaced \" }  ");
    cJSON_Minify(buf);
    cJSON *parsed = cJSON_Parse(buf);
    cJSON *min_item = cJSON_GetObjectItem(parsed, "min");
    const char *min_text = cJSON_GetStringValue(min_item);
    cJSON *min_str = cJSON_CreateString(min_text ? min_text : "");
    cJSON_AddItemToObject(root, "minified", min_str);

    // step 3: Operate & Validate
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    cJSON_bool third_is_null = cJSON_IsNull(third);
    cJSON *isnull_num = cJSON_CreateNumber((double)third_is_null);
    cJSON_AddItemToObject(root, "third_is_null", isnull_num);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}