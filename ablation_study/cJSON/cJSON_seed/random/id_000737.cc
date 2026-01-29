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
//<ID> 737
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *age_item = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *detached_name = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    char *printed = (char *)0;
    double age_val = 0.0;
    cJSON_bool added_name = 0;
    cJSON_bool added_items = 0;
    cJSON_bool added_age = 0;
    cJSON_bool added_flag = 0;
    int validation_score = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name_item = cJSON_CreateString("Bob");
    age_item = cJSON_CreateNumber(42.0);
    flag_item = cJSON_CreateTrue();

    // step 3: Configure
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_items = cJSON_AddItemToObject(root, "items", arr);
    added_age = cJSON_AddItemToArray(arr, age_item);
    added_flag = cJSON_AddItemToArray(arr, flag_item);

    // step 4: Operate
    retrieved_items = cJSON_GetObjectItem(root, "items");
    retrieved_name = cJSON_GetObjectItem(root, "name");
    detached_name = cJSON_DetachItemFromObject(root, "name");
    retrieved_name = cJSON_GetObjectItem(root, "name");
    printed = cJSON_PrintUnformatted(root);
    age_val = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_items, 0));
    (void)age_val;

    // step 5: Validate
    validation_score = (printed != (char *)0)
                     + (int)added_name + (int)added_items + (int)added_age + (int)added_flag
                     + (detached_name != (cJSON *)0)
                     + (int)cJSON_IsTrue(cJSON_GetArrayItem(retrieved_items, 1));
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached_name);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}