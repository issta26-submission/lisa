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
//<ID> 1135
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
    cJSON *fetched_elem0 = (cJSON *)0;
    char *name_str = (char *)0;
    char *elem0_str = (char *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object, a string member and an array with elements
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("sample_name");
    cJSON_AddItemToObject(root, "name", name_item);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    arr_elem0 = cJSON_CreateString("alpha");
    arr_elem1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, arr_elem0);
    cJSON_AddItemToArray(arr, arr_elem1);

    // step 3: Operate - retrieve object members and array element, extract strings, serialize
    fetched_name = cJSON_GetObjectItem(root, "name");
    fetched_values = cJSON_GetObjectItem(root, "values");
    fetched_elem0 = cJSON_GetArrayItem(fetched_values, 0);
    name_str = cJSON_GetStringValue(fetched_name);
    elem0_str = cJSON_GetStringValue(fetched_elem0);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Validate - perform lightweight validations and free serialized buffer
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(arr_elem0 != (cJSON *)0) + (int)(arr_elem1 != (cJSON *)0) + (int)(fetched_name != (cJSON *)0) + (int)(fetched_values != (cJSON *)0) + (int)(fetched_elem0 != (cJSON *)0) + (int)(name_str != (char *)0) + (int)(elem0_str != (char *)0);
    cJSON_free((void *)printed);
    (void)validation_score;

    // step 5: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}