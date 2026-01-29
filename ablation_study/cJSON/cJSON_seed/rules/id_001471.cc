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
//<ID> 1471
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
    cJSON *str0 = NULL;
    cJSON *num1 = NULL;
    cJSON *got_arr = NULL;
    cJSON *got_item0 = NULL;
    cJSON *dup_item = NULL;
    cJSON *detached_item = NULL;
    cJSON *retrieved_dup = NULL;
    cJSON *retrieved_num = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    str0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, str0);
    num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num1);

    // step 3: Operate / Validate
    got_arr = cJSON_GetObjectItem(root, "arr");
    got_item0 = cJSON_GetArrayItem(got_arr, 0);
    dup_item = cJSON_Duplicate(got_item0, 1);
    cJSON_AddItemToObject(root, "dup", dup_item);
    detached_item = cJSON_DetachItemViaPointer(got_arr, got_item0);
    cJSON_AddItemToObject(root, "moved", detached_item);
    retrieved_dup = cJSON_GetObjectItem(root, "dup");
    retrieved_num = cJSON_GetArrayItem(got_arr, 0);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)retrieved_dup;
    (void)retrieved_num;
    return 66;
}