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
//<ID> 282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const double numbers[4] = {3.14, 2.718, 0.0, -1.0};
    cJSON *root = cJSON_CreateObject();
    cJSON *nums_array = cJSON_CreateDoubleArray(numbers, 4);
    cJSON *label = cJSON_CreateString("dataset");

    // step 2: Configure
    cJSON_AddItemToObject(root, "numbers", nums_array);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");
    cJSON *has_numbers_item = cJSON_CreateBool(has_numbers);
    cJSON_AddItemToObject(root, "has_numbers", has_numbers_item);
    char *printed = cJSON_Print(root);
    cJSON *snapshot = cJSON_CreateString(printed ? printed : "");
    cJSON_AddItemToObject(root, "snapshot", snapshot);
    const char *snapshot_value = cJSON_GetStringValue(snapshot);
    (void)snapshot_value; // use the obtained string value symbolically

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}