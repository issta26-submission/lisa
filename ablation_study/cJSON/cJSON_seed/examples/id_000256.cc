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
//<ID> 256
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *objA = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(10.5);
    cJSON *num2 = cJSON_CreateNumber(0.0);
    const char *words[] = { "alpha", "beta", "gamma" };
    cJSON *strarr = cJSON_CreateStringArray(words, 3);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "objectA", objA);
    cJSON_AddItemToObjectCS(objA, "value", num1);
    cJSON_AddItemToObjectCS(root, "strings", strarr);
    cJSON_AddItemToObjectCS(root, "placeholder", num2);

    // step 3: Operate & Validate
    cJSON *val_ptr = cJSON_GetObjectItem(objA, "value");
    double val = cJSON_GetNumberValue(val_ptr);
    cJSON *double_num = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToObjectCS(root, "double_value", double_num);
    cJSON_bool has_strings = cJSON_HasObjectItem(root, "strings");
    cJSON *has_flag = cJSON_CreateNumber((double)has_strings);
    cJSON_AddItemToObjectCS(root, "has_strings_flag", has_flag);
    cJSON *place_ptr = cJSON_GetObjectItem(root, "placeholder");
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemViaPointer(root, place_ptr, replacement);
    cJSON *new_place = cJSON_GetObjectItem(root, "placeholder");
    double place_val = cJSON_GetNumberValue(new_place);
    cJSON *place_copy = cJSON_CreateNumber(place_val);
    cJSON_AddItemToObjectCS(root, "placeholder_copy", place_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}