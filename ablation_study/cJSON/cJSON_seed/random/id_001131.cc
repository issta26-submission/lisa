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
//<ID> 1131
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
    cJSON *obj_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *arr_elem0 = (cJSON *)0;
    cJSON *arr_elem1 = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    cJSON *fetched_elem1 = (cJSON *)0;
    cJSON *fetched_name_item = (cJSON *)0;
    char *printed = (char *)0;
    char *retrieved_name = (char *)0;
    char *retrieved_second = (char *)0;
    int validation_score = 0;
    cJSON_bool tmp_bool = (cJSON_bool)0;

    // step 2: Initialize - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    tmp_bool = cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create an object with a string and a number, and populate the array
    obj_item = cJSON_CreateObject();
    str_item = cJSON_CreateString("example");
    num_item = cJSON_CreateNumber(7.0);
    tmp_bool = cJSON_AddItemToObject(obj_item, "name", str_item);
    tmp_bool = cJSON_AddItemToObject(obj_item, "value", num_item);
    tmp_bool = cJSON_AddItemToArray(arr, obj_item);
    arr_elem0 = cJSON_CreateString("first");
    arr_elem1 = cJSON_CreateString("second");
    tmp_bool = cJSON_AddItemToArray(arr, arr_elem0);
    tmp_bool = cJSON_AddItemToArray(arr, arr_elem1);

    // step 4: Operate - serialize and retrieve items via object & array accessors
    printed = cJSON_PrintUnformatted(root);
    fetched_arr = cJSON_GetObjectItem(root, "items");
    fetched_elem0 = cJSON_GetArrayItem(fetched_arr, 0);
    fetched_elem1 = cJSON_GetArrayItem(fetched_arr, 1);
    fetched_name_item = cJSON_GetObjectItem(fetched_elem0, "name");
    retrieved_name = cJSON_GetStringValue(fetched_name_item);
    retrieved_second = cJSON_GetStringValue(fetched_elem1);

    // step 5: Validate - perform lightweight checks and free serialized buffer
    validation_score = (int)(retrieved_name != (char *)0) + (int)(retrieved_name[0] == 'e') + (int)(retrieved_second != (char *)0) + (int)(retrieved_second[0] == 'f');
    cJSON_free((void *)printed);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}