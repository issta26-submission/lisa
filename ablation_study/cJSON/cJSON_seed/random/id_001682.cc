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
//<ID> 1682
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_array = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    const char *strings[3];
    double num_val = 0.0;
    int validation = 0;

    // step 2: Initialize
    strings[0] = "alpha";
    strings[1] = "beta";
    strings[2] = "gamma";
    root = cJSON_CreateObject();
    str_array = cJSON_CreateStringArray(strings, 3);
    str_item = cJSON_CreateString("hello");

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", str_array);
    cJSON_AddItemToObject(root, "greeting", str_item);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);
    num_item = cJSON_AddNumberToObject(root, "value", 3.14);

    // step 4: Operate
    num_val = cJSON_GetNumberValue(num_item);

    // step 5: Validate and Cleanup
    validation = (int)(num_val == 3.14) + (int)(str_array != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(bool_item != (cJSON *)0);
    (void)validation;
    cJSON_Delete(root);
    (void)root;
    (void)str_array;
    (void)str_item;
    (void)num_item;
    (void)bool_item;
    (void)num_val;
    (void)strings;
    // API sequence test completed successfully
    return 66;
}