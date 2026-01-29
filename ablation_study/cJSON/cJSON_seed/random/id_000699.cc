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
//<ID> 699
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *obj1 = (cJSON *)0;
    cJSON *obj2 = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *shared = (cJSON *)0;
    cJSON *retr_child = (cJSON *)0;
    cJSON *retr_list = (cJSON *)0;
    cJSON *retr_shared = (cJSON *)0;
    cJSON_bool added_num = 0;
    cJSON_bool added_label = 0;
    cJSON_bool added_shared = 0;
    cJSON_bool added_child = 0;
    cJSON_bool added_list = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool is_list_array = 0;
    cJSON_bool replaced_ok = 0;
    double retrieved_number = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    obj1 = cJSON_CreateObject();
    obj2 = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(10.0);
    label = cJSON_CreateString("ten");
    shared = cJSON_CreateString("shared_value");

    // step 3: Configure
    added_num = cJSON_AddItemToObject(obj1, "num", num);
    added_label = cJSON_AddItemToObject(obj1, "label", label);
    added_shared = cJSON_AddItemToObject(root, "shared", shared);
    retr_shared = cJSON_GetObjectItem(root, "shared");
    added_ref = cJSON_AddItemReferenceToObject(obj1, "sharedRef", retr_shared);
    added_child = cJSON_AddItemToObject(root, "child", obj1);
    added_list = cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));

    // step 4: Operate
    retr_child = cJSON_GetObjectItem(root, "child");
    retr_list = cJSON_GetObjectItem(root, "list");
    is_list_array = cJSON_IsArray(retr_list);
    replaced_ok = cJSON_ReplaceItemViaPointer(root, retr_child, obj2);
    retrieved_number = cJSON_GetNumberValue(cJSON_GetArrayItem(retr_list, 0));

    // step 5: Validate
    (void)added_num;
    (void)added_label;
    (void)added_shared;
    (void)added_ref;
    (void)added_child;
    (void)added_list;
    (void)retr_shared;
    (void)retr_child;
    (void)retr_list;
    (void)is_list_array;
    (void)replaced_ok;
    (void)retrieved_number;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}