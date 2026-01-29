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
//<ID> 898
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *num_name = "value";
    const char *arr_name = "list";
    double expected_value = 3.14;
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *array_item = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *retrieved_value = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_num = (cJSON_bool)0;
    cJSON_bool added_list = (cJSON_bool)0;
    cJSON_bool added_e1 = (cJSON_bool)0;
    cJSON_bool added_e2 = (cJSON_bool)0;
    cJSON_bool is_array = (cJSON_bool)0;
    double got_number = 0.0;
    int validation_score = 0;

    // step 2: Initialize / Create items
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(expected_value);
    array_item = cJSON_CreateArray();
    elem1 = cJSON_CreateNumber(1.0);
    elem2 = cJSON_CreateNumber(2.0);

    // step 3: Configure object and array
    added_num = cJSON_AddItemToObject(root, num_name, num_item);
    added_list = cJSON_AddItemToObject(root, arr_name, array_item);
    added_e1 = cJSON_AddItemToArray(array_item, elem1);
    added_e2 = cJSON_AddItemToArray(array_item, elem2);

    // step 4: Operate: retrieve items and inspect
    retrieved_value = cJSON_GetObjectItem(root, num_name);
    retrieved_list = cJSON_GetObjectItem(root, arr_name);
    got_number = cJSON_GetNumberValue(retrieved_value);
    is_array = cJSON_IsArray(retrieved_list);

    // step 5: Serialize for inspection and compute lightweight validation score
    printed = cJSON_Print(root);
    validation_score = (int)(root != (cJSON *)0) + (int)added_num + (int)added_list + (int)added_e1 + (int)added_e2 + (int)is_array + (int)(printed != (char *)0) + (int)(got_number == expected_value);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}