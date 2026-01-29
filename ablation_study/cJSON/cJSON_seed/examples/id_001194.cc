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
//<ID> 1194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[1,2,3],\"flag\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(created_arr, n1);
    cJSON_AddItemToArray(created_arr, n2);
    cJSON_AddItemToObject(root, "created", created_arr);

    // step 3: Operate & Validate
    cJSON_ReplaceItemInArray(created_arr, 0, true_item);
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool equal = cJSON_Compare((const cJSON *const)true_item, (const cJSON *const)parsed_flag, 1);
    cJSON *match_bool = cJSON_CreateBool(equal);
    cJSON_AddItemToObject(root, "match", match_bool);
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *dup_numbers = cJSON_Duplicate(parsed_numbers, 1);
    cJSON_AddItemToObject(root, "orig_numbers", dup_numbers);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}