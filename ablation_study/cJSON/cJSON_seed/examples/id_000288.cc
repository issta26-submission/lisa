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
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3];
    numbers[0] = 1.5;
    numbers[1] = 2.5;
    numbers[2] = 3.5;
    cJSON *darr = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *label = cJSON_CreateString("example_label");

    // step 2: Configure
    cJSON_bool add_ok = cJSON_AddItemToObject(root, "values", darr);
    cJSON_bool label_ok = cJSON_AddItemToObject(root, "label", label);
    cJSON *add_ok_flag = cJSON_CreateBool(add_ok);
    cJSON_AddItemToObject(root, "added_flag", add_ok_flag);

    // step 3: Operate & Validate
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON *has_values_flag = cJSON_CreateBool(has_values);
    cJSON_AddItemToObject(root, "has_values", has_values_flag);
    char *printed = cJSON_Print(root);
    char *label_val = cJSON_GetStringValue((const cJSON *const)label);
    cJSON *label_copy = cJSON_CreateString(label_val);
    cJSON_AddItemToObject(root, "label_copy", label_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    // API sequence test completed successfully
    return 66;
}