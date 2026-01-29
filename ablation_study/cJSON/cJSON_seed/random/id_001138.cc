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
//<ID> 1138
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
    cJSON *arr_elem0 = (cJSON *)0;
    cJSON *arr_elem1 = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    cJSON *fetched_values = (cJSON *)0;
    cJSON *fetched_elem1 = (cJSON *)0;
    char *name_str = (char *)0;
    char *elem1_str = (char *)0;
    char *printed = (char *)0;
    cJSON_bool added_name = (cJSON_bool)0;
    cJSON_bool added_values = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name_item = cJSON_CreateString("root_name");
    arr_elem0 = cJSON_CreateString("alpha");
    arr_elem1 = cJSON_CreateString("beta");

    // step 3: Configure
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_values = cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, arr_elem0);
    cJSON_AddItemToArray(arr, arr_elem1);

    // step 4: Operate
    fetched_name = cJSON_GetObjectItem(root, "name");
    fetched_values = cJSON_GetObjectItem(root, "values");
    fetched_elem1 = cJSON_GetArrayItem(fetched_values, 1);
    name_str = cJSON_GetStringValue(fetched_name);
    elem1_str = cJSON_GetStringValue(fetched_elem1);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)added_name + (int)added_values + (int)(fetched_name != (cJSON *)0) + (int)(fetched_values != (cJSON *)0) + (int)(fetched_elem1 != (cJSON *)0) + (int)(name_str != (char *)0) + (int)(elem1_str != (char *)0);
    (void)validation_score;
    cJSON_free((void *)printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}