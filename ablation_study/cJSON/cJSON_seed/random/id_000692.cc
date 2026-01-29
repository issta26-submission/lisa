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
//<ID> 692
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
    cJSON *num = (cJSON *)0;
    cJSON *label_old = (cJSON *)0;
    cJSON *label_new = (cJSON *)0;
    cJSON *ref_label = (cJSON *)0;
    cJSON *retrieved_label = (cJSON *)0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_num_to_arr = 0;
    cJSON_bool added_label = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool replaced = 0;
    cJSON_bool is_array_flag = 0;
    const char *label_text = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(42.0);
    label_old = cJSON_CreateString("original");
    label_new = cJSON_CreateString("replacement");
    ref_label = cJSON_CreateString("referenced");

    // step 3: Configure
    added_arr_to_obj = cJSON_AddItemToObject(root, "list", arr);
    added_num_to_arr = cJSON_AddItemToArray(arr, num);
    added_label = cJSON_AddItemToObject(root, "label", label_old);
    added_ref = cJSON_AddItemReferenceToObject(root, "label_ref", label_old);
    cJSON_AddItemToObject(root, "extra_ref", ref_label);

    // step 4: Operate
    retrieved_label = cJSON_GetObjectItem(root, "label");
    replaced = cJSON_ReplaceItemViaPointer(root, retrieved_label, label_new);
    is_array_flag = cJSON_IsArray(cJSON_GetObjectItem(root, "list"));
    label_text = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));

    // step 5: Validate
    (void)added_arr_to_obj;
    (void)added_num_to_arr;
    (void)added_label;
    (void)added_ref;
    (void)replaced;
    (void)is_array_flag;
    (void)label_text;
    (void)retrieved_label;
    (void)ref_label;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}