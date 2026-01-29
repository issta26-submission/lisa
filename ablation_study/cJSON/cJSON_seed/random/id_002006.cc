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
//<ID> 2006
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *bool_item = NULL;
    cJSON_bool added_num_to_array = 0;
    cJSON_bool added_str_to_root = 0;
    cJSON_bool added_arr_to_root = 0;
    char *str_val = NULL;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup (Initialize root, array, number and string items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("hello world");

    // step 3: Configure (add items to structures; add a boolean to root)
    added_num_to_array = cJSON_AddItemToArray(arr, num_item);
    added_str_to_root = cJSON_AddItemToObject(root, "name", str_item);
    added_arr_to_root = cJSON_AddItemToObject(root, "numbers", arr);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate (inspect string value and array size)
    str_val = cJSON_GetStringValue(str_item);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate (aggregate simple checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(added_num_to_array != 0);
    validation ^= (int)(added_str_to_root != 0);
    validation ^= (int)(added_arr_to_root != 0);
    validation ^= (int)(str_val != NULL);
    validation ^= (arr_size & 0xFF);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}