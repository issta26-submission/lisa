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
//<ID> 1005
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_array = "data";
    const char *key_label = "label";
    const char *key_flag = "enabled";
    const char *text = "example_label";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON_bool add_ok_arr1 = (cJSON_bool)0;
    cJSON_bool add_ok_arr2 = (cJSON_bool)0;
    cJSON_bool add_ok_obj1 = (cJSON_bool)0;
    cJSON_bool add_ok_obj2 = (cJSON_bool)0;
    cJSON *fetched_first = (cJSON *)0;
    const char *retrieved_label = (const char *)0;
    char *serialized = (char *)0;
    double first_value = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root, array, string and true items and number items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString(text);
    true_item = cJSON_CreateTrue();
    n1 = cJSON_CreateNumber(7.0);
    n2 = cJSON_CreateNumber(11.0);

    // step 3: Configure - attach array and other items to root and populate the array
    add_ok_obj1 = cJSON_AddItemToObject(root, key_array, arr);
    add_ok_obj2 = cJSON_AddItemToObject(root, key_label, str_item);
    cJSON_bool add_ok_flag = cJSON_AddItemToObject(root, key_flag, true_item);
    add_ok_arr1 = cJSON_AddItemToArray(arr, n1);
    add_ok_arr2 = cJSON_AddItemToArray(arr, n2);

    // step 4: Operate - serialize and retrieve elements using API calls (including required GetArrayItem)
    serialized = cJSON_PrintUnformatted(root);
    fetched_first = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetNumberValue(fetched_first);
    retrieved_label = cJSON_GetStringValue(cJSON_GetObjectItem(root, key_label));

    // step 5: Validate - compute a simple validation score exercising returned values
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(n1 != (cJSON *)0) + (int)(n2 != (cJSON *)0) + (int)add_ok_obj1 + (int)add_ok_obj2 + (int)add_ok_arr1 + (int)add_ok_arr2 + (int)(serialized != (char *)0) + (int)(retrieved_label != (const char *)0 && retrieved_label[0] == 'e') + (int)(first_value == 7.0) + (int)add_ok_flag;
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which frees attached items)
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}