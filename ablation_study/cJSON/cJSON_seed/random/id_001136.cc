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
//<ID> 1136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *greeting_item = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *arr_elem0 = (cJSON *)0;
    cJSON *arr_elem1 = (cJSON *)0;
    cJSON *nested = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *fetched_greeting = (cJSON *)0;
    cJSON *fetched_list = (cJSON *)0;
    cJSON *fetched_first_elem = (cJSON *)0;
    cJSON *fetched_nested = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    char *greeting_str = (char *)0;
    char *first_elem_str = (char *)0;
    char *printed = (char *)0;
    double num_val = 0.0;
    int validation_score = 0;
    cJSON_bool add_ok = (cJSON_bool)0;

    // step 2: Initialize - create root object and members
    root = cJSON_CreateObject();
    greeting_item = cJSON_CreateString("hello");
    arr = cJSON_CreateArray();
    arr_elem0 = cJSON_CreateString("alpha");
    arr_elem1 = cJSON_CreateString("beta");
    nested = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(7.0);

    // step 3: Configure - assemble object tree using AddItemToObject and array population
    add_ok = cJSON_AddItemToObject(root, "greeting", greeting_item);
    cJSON_AddItemToArray(arr, arr_elem0);
    cJSON_AddItemToArray(arr, arr_elem1);
    add_ok = cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(nested, "num", num_item);
    add_ok = cJSON_AddItemToObject(root, "nested", nested);
    (void)add_ok;

    // step 4: Operate - retrieve items and array elements, extract values
    fetched_greeting = cJSON_GetObjectItem(root, "greeting");
    greeting_str = cJSON_GetStringValue(fetched_greeting);
    fetched_list = cJSON_GetObjectItem(root, "list");
    fetched_first_elem = cJSON_GetArrayItem(fetched_list, 0);
    first_elem_str = cJSON_GetStringValue(fetched_first_elem);
    fetched_nested = cJSON_GetObjectItem(root, "nested");
    fetched_num = cJSON_GetObjectItem(fetched_nested, "num");
    num_val = cJSON_GetNumberValue(fetched_num);

    // step 5: Validate - serialize, compute simple validation score, free serialized buffer
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(greeting_str != (char *)0) + (int)(first_elem_str != (char *)0) + (int)(num_val == 7.0);
    cJSON_free((void *)printed);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}