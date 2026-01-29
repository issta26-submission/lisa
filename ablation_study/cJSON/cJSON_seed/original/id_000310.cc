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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "values");
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(-7.5);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON *raw = cJSON_CreateRaw("{\"x\":1,\"y\":2}");
    cJSON_AddItemToObject(root, "raw_blob", raw);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Configure
    cJSON_bool is_raw = cJSON_IsRaw(raw);
    (void)is_raw;
    cJSON_bool is_invalid_root = cJSON_IsInvalid(root);
    (void)is_invalid_root;
    double first_value = cJSON_GetNumberValue(num1);
    cJSON *dbl = cJSON_CreateNumber(first_value * 2.0);
    cJSON_AddItemToObject(root, "double_first", dbl);

    // step 3: Operate and Validate
    const char *greet_str = cJSON_GetStringValue(greeting);
    (void)greet_str;
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}