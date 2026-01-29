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
//<ID> 835
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[1,2,3], \"flag\": false, \"raw\":\"<raw>\"}";
    size_t json_len = (size_t)strlen(json_text);
    cJSON *parsed = cJSON_ParseWithLength(json_text, json_len);
    cJSON *root = cJSON_CreateObject();
    cJSON *generated_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "generated", generated_array);

    // step 2: Configure
    cJSON_AddRawToObject(root, "embedded_raw", "{\"embedded\":true,\"x\":42}");
    cJSON_bool arr_is_array = cJSON_IsArray(generated_array);
    cJSON_AddBoolToObject(root, "generated_is_array", arr_is_array);
    cJSON *numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *numbers_ref = cJSON_CreateArrayReference(numbers);
    cJSON_AddItemToObject(root, "numbers_ref", numbers_ref);

    // step 3: Operate
    cJSON_AddItemToArray(generated_array, cJSON_CreateNumber(9.5));
    cJSON_AddItemToArray(generated_array, cJSON_CreateString("generated"));
    char *compact = cJSON_PrintUnformatted(root);
    double first_num = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 0));
    (void)first_num;
    cJSON_free(compact);

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}