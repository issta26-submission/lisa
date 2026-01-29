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
//<ID> 996
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device-XYZ");
    int numbers_data[3] = {10, 20, 30};
    cJSON *numbers = cJSON_CreateIntArray(numbers_data, 3);

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObjectCS(root, "numbers", numbers);

    // step 3: Operate & Validate
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(got_name);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    int arr_size = cJSON_GetArraySize(numbers);
    cJSON *count = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "numbers_count", count);
    cJSON_bool present = cJSON_HasObjectItem(root, "numbers");
    cJSON *present_flag = cJSON_CreateBool(present);
    cJSON_AddItemToObjectCS(root, "has_numbers", present_flag);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}