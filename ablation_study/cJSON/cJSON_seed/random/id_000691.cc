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
//<ID> 691
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *newarr = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *newelem = (cJSON *)0;
    cJSON *ref_only = (cJSON *)0;
    cJSON *retrieved_before = (cJSON *)0;
    cJSON *retrieved_after = (cJSON *)0;
    cJSON_bool was_array_before = 0;
    cJSON_bool was_array_after = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool replaced = 0;
    int size_before = 0;
    int size_after = 0;
    int presence_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem1 = cJSON_CreateString("one");
    elem2 = cJSON_CreateString("two");

    // step 3: Configure
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    added_arr_to_root = cJSON_AddItemToObject(root, "items", arr);
    ref_only = cJSON_CreateString("reference-only");
    added_ref = cJSON_AddItemReferenceToObject(root, "ref_item", ref_only);
    newarr = cJSON_CreateArray();
    newelem = cJSON_CreateString("replacement");
    cJSON_AddItemToArray(newarr, newelem);

    // step 4: Operate
    retrieved_before = cJSON_GetObjectItem(root, "items");
    was_array_before = cJSON_IsArray(retrieved_before);
    size_before = cJSON_GetArraySize(retrieved_before);
    replaced = cJSON_ReplaceItemViaPointer(root, retrieved_before, newarr);
    retrieved_after = cJSON_GetObjectItem(root, "items");
    was_array_after = cJSON_IsArray(retrieved_after);
    size_after = cJSON_GetArraySize(retrieved_after);

    // step 5: Validate
    presence_count = (int)was_array_before + (int)was_array_after + added_arr_to_root + added_ref + replaced + size_before + size_after;
    (void)presence_count;
    (void)elem1;
    (void)elem2;
    (void)ref_only;
    (void)newelem;
    (void)retrieved_before;
    (void)retrieved_after;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}