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
//<ID> 2005
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
    char *str_val = NULL;
    int arr_size = 0;
    cJSON_bool added_num = 0;
    cJSON_bool added_str = 0;
    cJSON_bool added_arr = 0;
    int validation = 0;

    // step 2: Setup (initialize root, array, and items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(3.14159);
    str_item = cJSON_CreateString("sample");

    // step 3: Core operations (assemble JSON and use required APIs)
    added_num = cJSON_AddItemToArray(arr, num_item);
    added_str = cJSON_AddItemToArray(arr, str_item);
    added_arr = cJSON_AddItemToObject(root, "values", arr);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);
    arr_size = cJSON_GetArraySize(arr);
    str_val = (char *)cJSON_GetStringValue(str_item);

    // step 4: Validate (aggregate simple checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(added_num != 0);
    validation ^= (int)(added_str != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(str_val != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}