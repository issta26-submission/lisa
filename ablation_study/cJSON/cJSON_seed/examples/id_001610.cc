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
//<ID> 1610
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s0 = cJSON_CreateString("first");
    cJSON *s1 = cJSON_CreateString("second");
    cJSON *s2 = cJSON_CreateString("third");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 2: Configure (insert, replace, detach, re-insert)
    cJSON *inserted = cJSON_CreateString("inserted_at_1");
    cJSON_bool ins_ok = cJSON_InsertItemInArray(arr, 1, inserted); (void)ins_ok;
    cJSON *replacement = cJSON_CreateString("replaced_at_2");
    cJSON_bool rep_ok = cJSON_ReplaceItemInArray(arr, 2, replacement); (void)rep_ok;
    cJSON *detached0 = cJSON_DetachItemFromArray(arr, 0);
    cJSON_bool rein_ok = cJSON_InsertItemInArray(arr, 3, detached0); (void)rein_ok;

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "items_count", count_num);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}