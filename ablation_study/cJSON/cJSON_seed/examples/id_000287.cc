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
//<ID> 287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[4] = {3.14159, 2.71828, 1.41421, 0.57721};
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 4);
    cJSON *greeting = cJSON_CreateString("hello_cjson");

    // step 2: Configure
    cJSON_AddItemToObject(root, "constants", double_array);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_bool has_constants = cJSON_HasObjectItem(root, "constants");
    cJSON *has_constants_flag = cJSON_CreateBool(has_constants);
    cJSON_AddItemToObject(root, "has_constants", has_constants_flag);

    // step 3: Operate & Validate
    const char *greeting_text = cJSON_GetStringValue(greeting);
    cJSON *greeting_copy = cJSON_CreateString(greeting_text ? greeting_text : "");
    cJSON_AddItemToObject(root, "greeting_copy", greeting_copy);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}