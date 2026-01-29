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
//<ID> 995
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int initial_numbers[] = {10, 20, 30, 40};
    int initial_count = 4;
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateIntArray(initial_numbers, initial_count);
    cJSON *label = cJSON_CreateString("sensor-A");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "numbers", nums);
    cJSON_AddItemToObjectCS(root, "label", label);
    cJSON_AddItemToObjectCS(root, "info", info);
    int array_size = cJSON_GetArraySize(nums);
    cJSON *numbers_count = cJSON_CreateNumber((double)array_size);
    cJSON_AddItemToObjectCS(root, "numbers_count", numbers_count);
    char *label_str = cJSON_GetStringValue(label);
    cJSON *label_copy = cJSON_CreateString(label_str);
    cJSON_AddItemToObjectCS(info, "label_copy", label_copy);
    cJSON_bool has_nums = cJSON_HasObjectItem(root, "numbers");
    cJSON *has_flag = cJSON_CreateNumber((double)has_nums);
    cJSON_AddItemToObjectCS(root, "has_numbers_flag", has_flag);

    // step 3: Operate & Validate
    cJSON *got_count = cJSON_GetObjectItem(root, "numbers_count");
    double current_count = cJSON_GetNumberValue(got_count);
    double updated_count = cJSON_SetNumberHelper(got_count, current_count + (double)has_nums);
    (void)updated_count;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}