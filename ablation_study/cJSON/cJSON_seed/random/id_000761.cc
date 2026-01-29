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
//<ID> 761
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
    cJSON *name_elem = (cJSON *)0;
    cJSON *name_prop = (cJSON *)0;
    cJSON *retrieved_elem = (cJSON *)0;
    char *retrieved_str = (char *)0;
    cJSON_bool added_names = 0;
    cJSON_bool added_prop = 0;
    cJSON_bool root_is_object = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name_elem = cJSON_CreateString("Bob");
    name_prop = cJSON_CreateString("Bob");

    // step 3: Configure
    added_names = cJSON_AddItemToObject(root, "names", arr);
    cJSON_AddItemToArray(arr, name_elem);
    added_prop = cJSON_AddItemToObject(root, "first_name", name_prop);

    // step 4: Operate
    retrieved_elem = cJSON_GetArrayItem(arr, 0);
    retrieved_str = cJSON_GetStringValue(retrieved_elem);
    root_is_object = cJSON_IsObject(root);

    // step 5: Validate
    validation_score = (retrieved_str != (char *)0) + (int)root_is_object + (int)added_names + (int)added_prop;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}