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
//<ID> 382
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[10,20],\"flag\":false}";
    const char *parse_end = nullptr;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_bool has_numbers = cJSON_HasObjectItem(parsed, "numbers");
    cJSON *has_numbers_item = cJSON_CreateBool(has_numbers);
    cJSON_AddItemToObject(root, "has_numbers", has_numbers_item);
    const char *version = cJSON_Version();
    cJSON *version_item = cJSON_CreateString(version);
    cJSON_AddItemToObject(root, "cjson_version", version_item);
    cJSON *copied_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "copied_numbers", copied_array);

    // step 3: Operate and Validate
    cJSON *numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *num0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *num1 = cJSON_GetArrayItem(numbers, 1);
    double val0 = cJSON_GetNumberValue(num0);
    double val1 = cJSON_GetNumberValue(num1);
    cJSON *new0 = cJSON_CreateNumber(val0 + 1.0);
    cJSON *new1 = cJSON_CreateNumber(val1 + 1.0);
    cJSON_AddItemToArray(copied_array, new0);
    cJSON_AddItemToArray(copied_array, new1);
    double total = val0 + val1;
    cJSON_AddNumberToObject(root, "sum", total);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}