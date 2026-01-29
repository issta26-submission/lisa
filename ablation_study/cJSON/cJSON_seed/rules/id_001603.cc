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
//<ID> 1603
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *elem = NULL;
    cJSON *raw = NULL;
    cJSON *true_item = NULL;
    cJSON *arr_ref = NULL;
    cJSON *detached = NULL;
    cJSON *got_flag = NULL;
    int arr_size = 0;
    cJSON_bool flag_is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "my_array", arr);
    elem = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, elem);
    raw = cJSON_CreateRaw("{\"inner\":42}");
    cJSON_AddItemToObject(root, "raw_blob", raw);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate / Validate
    arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root, "array_ref", arr_ref);
    detached = cJSON_DetachItemFromObject(root, "raw_blob");
    cJSON_AddItemToObject(root, "raw_readded", detached);
    arr_size = cJSON_GetArraySize(arr);
    got_flag = cJSON_GetObjectItem(root, "flag");
    flag_is_true = cJSON_IsTrue(got_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}