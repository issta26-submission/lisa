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
//<ID> 114
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
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *raw_item = cJSON_CreateRaw("{\"rawkey\":true}");
    cJSON_AddItemToArray(arr, raw_item);

    // step 2: Configure
    char *json_buf = (char *)cJSON_malloc(64);
    memset(json_buf, 0, 64);
    strcpy(json_buf, "  [  null  ,  42  ,  \"world\"  ]  ");
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);

    // step 3: Operate & Validate
    cJSON *first_elem = cJSON_GetArrayItem(parsed, 0);
    cJSON_bool first_was_null = cJSON_IsNull(first_elem);
    cJSON *flag_num = cJSON_CreateNumber((double)first_was_null);
    cJSON_AddItemToObject(root, "first_is_null", flag_num);
    cJSON *second_elem = cJSON_GetArrayItem(parsed, 1);
    double second_val = cJSON_GetNumberValue(second_elem);
    cJSON *second_copy = cJSON_CreateNumber(second_val);
    cJSON_AddItemToObject(root, "second_copy", second_copy);
    cJSON_AddItemToObject(root, "parsed", parsed);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(json_buf);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}