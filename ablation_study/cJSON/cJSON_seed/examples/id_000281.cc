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
//<ID> 281
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *num_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON *name_item = cJSON_CreateString("example_name");
    const char *version_str = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version_str);

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", num_array);
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "version", version_ref);

    // step 3: Operate & Validate
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");
    cJSON *has_numbers_flag = cJSON_CreateBool(has_numbers);
    cJSON_AddItemToObject(root, "has_numbers", has_numbers_flag);
    char *printed = cJSON_Print(root);
    const char *name_val = cJSON_GetStringValue(name_item);
    cJSON *name_ref = cJSON_CreateStringReference(name_val);
    cJSON_AddItemToObject(root, "name_ref", name_ref);
    cJSON *printed_copy = cJSON_CreateString(printed);
    cJSON_AddItemToObject(root, "printed_copy", printed_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    // API sequence test completed successfully
    return 66;
}