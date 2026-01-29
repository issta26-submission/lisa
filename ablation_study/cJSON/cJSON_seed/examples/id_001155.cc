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
//<ID> 1155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *obj_text = "{\"name\":\"test\",\"active\":true,\"count\":3}";
    const char *arr_text = "[1,2,3]";
    cJSON *parsed_obj = cJSON_Parse(obj_text);
    cJSON *parsed_arr = cJSON_Parse(arr_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *dup_obj = cJSON_Duplicate(parsed_obj, 1);
    cJSON *arr_dup = cJSON_Duplicate(parsed_arr, 1);
    cJSON *injected_name = cJSON_CreateString("injected");
    cJSON *initial_flag = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "original", cJSON_Duplicate(parsed_obj, 1));
    cJSON_AddItemToObject(root, "duplicated", dup_obj);
    cJSON_AddItemToObject(root, "array_copy", arr_dup);
    cJSON_AddItemToObject(root, "injected_name", injected_name);
    cJSON_AddItemToObject(root, "initial_flag", initial_flag);

    // step 3: Operate & Validate
    cJSON_bool equal = cJSON_Compare(parsed_obj, dup_obj, 1);
    cJSON *compare_flag = cJSON_CreateBool(equal);
    cJSON_AddItemToObject(root, "compare_result", compare_flag);
    cJSON *label = cJSON_CreateString("comparison_performed");
    cJSON_AddItemToObject(root, "label", label);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed_obj);
    cJSON_Delete(parsed_arr);
    // API sequence test completed successfully
    return 66;
}