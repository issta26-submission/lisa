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
//<ID> 2176
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
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *arr = NULL;
    cJSON *helper_bool = NULL;
    char *printed = NULL;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added3 = 0;
    cJSON_bool is_array = 0;
    int validation = 0;

    // step 2: Setup (create object, booleans, and array)
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    added1 = cJSON_AddItemToObject(root, "explicitTrue", true_item);
    added2 = cJSON_AddItemToObject(root, "explicitFalse", false_item);
    helper_bool = cJSON_AddBoolToObject(root, "helperBool", 1);
    arr = cJSON_CreateArray();
    added3 = cJSON_AddItemToObject(root, "array", arr);

    // step 3: Operate (inspect and serialize)
    is_array = cJSON_IsArray(arr);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(helper_bool != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(added3 != 0);
    validation ^= (int)(is_array != 0);
    validation ^= (int)(printed != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}