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
//<ID> 2175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *added_bool_item = NULL;
    cJSON *arr = NULL;
    cJSON *arr_elem = NULL;
    cJSON *got_arr = NULL;
    char *printed = NULL;
    cJSON_bool added_explicit_true = 0;
    cJSON_bool added_explicit_false = 0;
    cJSON_bool added_array_elem = 0;
    cJSON_bool is_array = 0;
    int validation = 0;

    // step 2: Setup (initialize root, booleans, and an array)
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    added_explicit_true = cJSON_AddItemToObject(root, "explicitTrue", true_item);
    added_explicit_false = cJSON_AddItemToObject(root, "explicitFalse", false_item);
    added_bool_item = cJSON_AddBoolToObject(root, "addedBoolTrue", 1);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    arr_elem = cJSON_CreateNumber(123.0);
    added_array_elem = cJSON_AddItemToArray(arr, arr_elem);

    // step 3: Operate (serialize and inspect)
    printed = cJSON_PrintUnformatted(root);
    got_arr = cJSON_GetObjectItem(root, "items");
    is_array = cJSON_IsArray(got_arr);

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(added_bool_item != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(arr_elem != NULL);
    validation ^= (int)(added_explicit_true != 0);
    validation ^= (int)(added_explicit_false != 0);
    validation ^= (int)(added_array_elem != 0);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_arr != NULL);
    validation ^= (int)(is_array != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}