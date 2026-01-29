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
//<ID> 1199
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[10,20,30],\"meta\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *explicit_true = cJSON_CreateTrue();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);

    // step 2: Configure
    cJSON_AddItemToArray(created_arr, n0);
    cJSON_AddItemToArray(created_arr, n1);
    cJSON_AddItemToArray(created_arr, n2);
    cJSON_AddItemToObject(root, "created", created_arr);
    cJSON_AddItemToObject(root, "explicit_true", explicit_true);

    // step 3: Operate & Validate
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *parsed_item1 = cJSON_GetArrayItem(parsed_arr, 1);
    cJSON *dup_item = cJSON_Duplicate(parsed_item1, 1);
    cJSON_ReplaceItemInArray(created_arr, 1, dup_item);
    cJSON_bool equal = cJSON_Compare(parsed_item1, dup_item, 1);
    cJSON *compare_flag = cJSON_CreateBool(equal);
    cJSON_AddItemToObject(root, "compare_equal", compare_flag);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}