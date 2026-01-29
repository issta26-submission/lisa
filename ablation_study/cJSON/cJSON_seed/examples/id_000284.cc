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
//<ID> 284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = {1.23, 4.56, 7.89};
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *name = cJSON_CreateString("example_label");
    cJSON_AddItemToObject(root, "values", double_array);
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure & Operate
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON *has_values_item = cJSON_CreateBool(has_values);
    cJSON_AddItemToObject(root, "has_values", has_values_item);
    char *name_value = cJSON_GetStringValue(name);
    cJSON *name_copy = cJSON_CreateString(name_value);
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 3: Print & Validate
    char *printed_json = cJSON_Print(root);
    cJSON *printed_item = cJSON_CreateString(printed_json);
    cJSON_AddItemToObject(root, "printed", printed_item);
    cJSON_free(printed_json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}