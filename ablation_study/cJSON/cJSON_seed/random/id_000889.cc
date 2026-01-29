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
//<ID> 889
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *obj_name_label = "label";
    const char *string_value = "hello world";
    const char *bool_name = "active";
    const char *number_name = "count";
    double number_value = 42.0;
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *number_item = (cJSON *)0;
    cJSON_bool added_label = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize root object and create a string item
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(string_value);

    // step 3: Configure object by adding a true, a number, and the created string
    true_item = cJSON_AddTrueToObject(root, bool_name);
    number_item = cJSON_AddNumberToObject(root, number_name, number_value);
    added_label = cJSON_AddItemToObject(root, obj_name_label, str_item);

    // step 4: Operate / Validate by aggregating simple checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)added_label + (int)(true_item != (cJSON *)0) + (int)(number_item != (cJSON *)0);
    (void)validation_score;

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}