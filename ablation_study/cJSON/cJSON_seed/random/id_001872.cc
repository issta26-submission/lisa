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
//<ID> 1872
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
    cJSON *arr = NULL;
    cJSON *str0 = NULL;
    cJSON *str1 = NULL;
    cJSON *bool_item = NULL;
    cJSON *got0 = NULL;
    cJSON_bool arr_check = 0;
    cJSON_bool got0_is_array = 0;
    const char *got0_value = NULL;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    str0 = cJSON_CreateString("first_item");
    str1 = cJSON_CreateString("second_item");
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Core operations
    got0 = cJSON_GetArrayItem(arr, 0);
    arr_check = cJSON_IsArray(arr);
    got0_is_array = cJSON_IsArray(got0);
    got0_value = (got0 != NULL) ? cJSON_GetStringValue(got0) : NULL;

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(str0 != NULL);
    validation ^= (int)(str1 != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(arr_check != 0);
    validation ^= (int)(got0_is_array == 0);
    validation ^= (int)(got0_value != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}