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
//<ID> 1689
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *str_arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    const char *strings[3] = { "one", "two", "three" };
    double extracted_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello");
    str_arr = cJSON_CreateStringArray(strings, 3);
    num_item = cJSON_CreateNumber(123.0);

    // step 3: Configure
    cJSON_AddItemToObject(root, "single", str_item);
    cJSON_AddItemToObject(root, "list", str_arr);
    cJSON_AddItemToObject(root, "value", num_item);
    bool_item = cJSON_AddBoolToObject(root, "flag", 1);

    // step 4: Operate
    extracted_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));

    // step 5: Validate
    validation = (int)(extracted_value == 123.0) + (int)(bool_item != (cJSON *)0) + (int)(cJSON_GetArraySize(str_arr) == 3);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)str_item;
    (void)str_arr;
    (void)num_item;
    (void)bool_item;
    (void)extracted_value;
    // API sequence test completed successfully
    return 66;
}