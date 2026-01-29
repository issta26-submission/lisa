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
//<ID> 700
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_bool added_arr = cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *truth = cJSON_CreateTrue();
    cJSON_bool added1 = cJSON_AddItemToArray(arr, str1);
    cJSON_bool added2 = cJSON_AddItemToArray(arr, truth);
    cJSON *num_added = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate
    cJSON *str2 = cJSON_CreateString("beta");
    cJSON_bool added3 = cJSON_AddItemToArray(arr, str2);
    char *serialized = cJSON_PrintUnformatted(root);

    // step 4: Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    cJSON *it2 = cJSON_GetArrayItem(arr, 2);
    const char *s0 = cJSON_GetStringValue(it0);
    cJSON_bool is_true = cJSON_IsTrue(it1);
    const char *s2 = cJSON_GetStringValue(it2);
    int validation = 0;
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(str1 != NULL);
    validation ^= (int)(truth != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(num_added != NULL);
    validation ^= (int)(serialized != NULL);
    validation ^= (int)(size == 3);
    validation ^= (int)(s0 != NULL && s0[0] == 'a');
    validation ^= (int)(is_true != 0);
    validation ^= (int)(s2 != NULL && s2[0] == 'b');
    (void)validation;

    // step 5: Cleanup
    cJSON_free((void*)serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}