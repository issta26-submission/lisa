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
//<ID> 2070
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
    cJSON *array = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    void *scratch = NULL;
    int arr_size = 0;
    cJSON_bool is_true0 = 0;
    cJSON_bool is_true1 = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    scratch = cJSON_malloc(16);
    ((char *)scratch)[0] = 'x';

    // step 3: Configure
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(array, true_item);
    cJSON_AddItemToArray(array, false_item);
    cJSON_AddItemToObject(root, "flags", array);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(array);
    got0 = cJSON_GetArrayItem(array, 0);
    got1 = cJSON_GetArrayItem(array, 1);
    is_true0 = cJSON_IsTrue(got0);
    is_true1 = cJSON_IsTrue(got1);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(array != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(scratch != NULL);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(is_true0 != 0);
    validation ^= (int)(is_true1 == 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}