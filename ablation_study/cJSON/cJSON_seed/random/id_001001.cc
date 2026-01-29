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
//<ID> 1001
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "items";
    const char *msg_key = "message";
    const char *status_key = "status";
    const char *text = "hello";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *msg_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON_bool add_arr_to_root = (cJSON_bool)0;
    cJSON_bool add_msg_to_root = (cJSON_bool)0;
    cJSON_bool add_status_to_root = (cJSON_bool)0;
    cJSON_bool add_str_to_array = (cJSON_bool)0;
    cJSON *fetched0 = (cJSON *)0;
    const char *fetched_str = (const char *)0;
    cJSON *status_item = (cJSON *)0;
    cJSON_bool is_status_true = (cJSON_bool)0;
    char *serialized = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, a string item and a true boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString(text);
    msg_item = cJSON_CreateString("greeting");
    true_item = cJSON_CreateTrue();

    // step 3: Configure - attach the array and other items to the root and populate the array
    add_arr_to_root = cJSON_AddItemToObject(root, array_key, arr);
    add_msg_to_root = cJSON_AddItemToObject(root, msg_key, msg_item);
    add_status_to_root = cJSON_AddItemToObject(root, status_key, true_item);
    add_str_to_array = cJSON_AddItemToArray(arr, str_item);

    // step 4: Operate - retrieve the first array element and inspect the status item
    fetched0 = cJSON_GetArrayItem(arr, 0);
    fetched_str = cJSON_GetStringValue(fetched0);
    status_item = cJSON_GetObjectItem(root, status_key);
    is_status_true = cJSON_IsTrue(status_item);
    serialized = cJSON_PrintUnformatted(root);

    // step 5: Validate - compute a small validation score exercising returned values
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(msg_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(add_arr_to_root) + (int)(add_msg_to_root) + (int)(add_status_to_root) + (int)(add_str_to_array) + (int)(fetched0 != (cJSON *)0) + (int)(fetched_str != (const char *)0 && fetched_str[0] == 'h') + (int)(is_status_true);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root which frees attached items
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}