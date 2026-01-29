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
//<ID> 110
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
    cJSON_AddItemToObject(root, "array", arr);

    // step 2: Configure
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, null_item);
    cJSON *raw_item = cJSON_CreateRaw("{\"raw_key\": 123}");
    cJSON_AddItemToArray(arr, raw_item);
    size_t buf_size = 64;
    char *json_buf = (char *)cJSON_malloc(buf_size);
    memset(json_buf, 0, buf_size);
    strcpy(json_buf, "   {  \"parsed_key\"  :  \"parsed value\"  }   ");
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_null = cJSON_IsNull(first);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_null = cJSON_IsNull(second);
    cJSON *flag1 = cJSON_CreateBool(first_is_null);
    cJSON *flag2 = cJSON_CreateBool(second_is_null);
    cJSON_AddItemToObject(root, "first_is_null", flag1);
    cJSON_AddItemToObject(root, "second_is_null", flag2);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(json_buf);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}