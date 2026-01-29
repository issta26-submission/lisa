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
//<ID> 729
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *age_item = (cJSON *)0;
    cJSON *active_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *retrieved_age = (cJSON *)0;
    char *name_str = (char *)0;
    double age_val = 0.0;
    char *printed = (char *)0;
    cJSON_bool added_name = 0;
    cJSON_bool added_age = 0;
    cJSON_bool added_active = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("Alice");
    age_item = cJSON_CreateNumber(30.5);
    active_item = cJSON_CreateFalse();

    // step 3: Configure
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_age = cJSON_AddItemToObject(root, "age", age_item);
    added_active = cJSON_AddItemToObject(root, "active", active_item);

    // step 4: Operate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    retrieved_age = cJSON_GetObjectItem(root, "age");
    name_str = cJSON_GetStringValue(retrieved_name);
    age_val = cJSON_GetNumberValue(retrieved_age);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (printed != (char *)0)
                     + (int)added_name + (int)added_age + (int)added_active
                     + (name_str != (char *)0) + (age_val > 0.0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}