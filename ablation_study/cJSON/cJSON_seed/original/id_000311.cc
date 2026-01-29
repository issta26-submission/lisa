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
//<ID> 311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, str);

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"x\":1}");
    cJSON_AddItemToObject(root, "raw_blob", raw);
    cJSON *nested = cJSON_AddArrayToObject(root, "nested");
    cJSON *num2 = cJSON_CreateNumber(-3.14);
    cJSON_AddItemToArray(nested, num2);

    // step 3: Operate and Validate
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw);
    const char *str_val = cJSON_GetStringValue(str);
    cJSON_bool num_is_invalid = cJSON_IsInvalid(num);
    cJSON_AddBoolToObject(root, "raw_is_raw", raw_is_raw);
    cJSON_AddBoolToObject(root, "num_is_invalid", num_is_invalid);
    cJSON_AddStringToObject(root, "extracted_string", str_val);
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