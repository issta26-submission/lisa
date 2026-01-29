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
//<ID> 839
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data_arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data_arr);
    const char parsed_text[] = "[{\"id\":1}, {\"id\":2}]";
    size_t parsed_len = sizeof(parsed_text) - 1;
    cJSON *parsed = cJSON_ParseWithLength(parsed_text, parsed_len);

    // step 2: Configure
    cJSON_AddRawToObject(root, "raw_blob", "{\"raw_key\": \"raw_val\", \"num\": 7}");
    cJSON_AddItemToArray(data_arr, cJSON_Duplicate(parsed, 1));
    cJSON_AddItemToArray(data_arr, cJSON_CreateNumber(3.14));
    cJSON_AddBoolToObject(root, "feature_enabled", 1);

    // step 3: Operate
    cJSON_bool is_array = cJSON_IsArray(data_arr);
    cJSON_AddBoolToObject(root, "data_is_array", is_array);
    char *out = cJSON_PrintUnformatted(root);
    double first_num = cJSON_GetNumberValue(cJSON_GetArrayItem(data_arr, 1));
    (void)first_num;

    // step 4: Validate & Cleanup
    int size = cJSON_GetArraySize(data_arr);
    (void)size;
    cJSON_free(out);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}