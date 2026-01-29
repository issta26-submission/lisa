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
//<ID> 2003
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
    cJSON *got_item = NULL;
    cJSON *bool_item = NULL;
    cJSON_bool added_arr = 0;
    cJSON_bool added_num = 0;
    cJSON_bool added_str = 0;
    int arr_size = 0;
    int validation = 0;
    char *extracted_str = NULL;

    // step 2: Setup (initialize root, array, and items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    added_arr = cJSON_AddItemToObject(root, "items", arr);
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("example");
    added_num = cJSON_AddItemToArray(arr, num_item);
    added_str = cJSON_AddItemToArray(arr, str_item);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate (inspect array and extract string)
    arr_size = cJSON_GetArraySize(arr);
    got_item = cJSON_GetArrayItem(arr, 1);
    extracted_str = (char *)cJSON_GetStringValue(got_item);

    // step 4: Validate (aggregate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(added_num != 0);
    validation ^= (int)(added_str != 0);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(extracted_str != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}