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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const fruits[] = { "apple", "banana", "cherry" };
    cJSON *fruit_array = cJSON_CreateStringArray(fruits, 3);
    cJSON_AddItemToObject(root, "fruits", fruit_array);

    // step 2: Configure
    char jsonbuf[] = "{ \"status\" : \"ok\"    \n}";
    cJSON_Minify(jsonbuf);
    cJSON *minified_str = cJSON_CreateString(jsonbuf);
    cJSON_AddItemToObject(root, "minified", minified_str);

    // step 3: Operate & Validate
    cJSON_bool has_fruits = cJSON_HasObjectItem(root, "fruits");
    cJSON_AddItemToObject(root, "has_fruits", cJSON_CreateBool(has_fruits));
    cJSON *second = cJSON_GetArrayItem(fruit_array, 1);
    const char *second_text = cJSON_GetStringValue(second);
    cJSON *picked = cJSON_CreateString(second_text);
    cJSON_AddItemToObject(root, "picked", picked);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}