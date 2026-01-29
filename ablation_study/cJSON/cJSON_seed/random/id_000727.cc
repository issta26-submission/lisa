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
//<ID> 727
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *copied_str_item = (cJSON *)0;
    char *name_value = (char *)0;
    double number_value = 0.0;
    cJSON_bool added_value = 0;
    cJSON_bool added_name = 0;
    cJSON_bool added_enabled = 0;
    cJSON_bool added_copy = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.1415);
    str_item = cJSON_CreateString("pi");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_value = cJSON_AddItemToObject(root, "value", num_item);
    added_name = cJSON_AddItemToObject(root, "name", str_item);
    added_enabled = cJSON_AddItemToObject(root, "enabled", false_item);

    // step 4: Operate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(retrieved_name);
    copied_str_item = cJSON_CreateString(name_value);
    added_copy = cJSON_AddItemToObject(root, "name_copy", copied_str_item);
    number_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));

    // step 5: Validate
    validation_score = (int)(name_value != (char *)0)
                     + (int)added_value + (int)added_name + (int)added_enabled + (int)added_copy
                     + (int)(number_value > 0.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}