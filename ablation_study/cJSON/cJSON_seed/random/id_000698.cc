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
//<ID> 698
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *objA = (cJSON *)0;
    cJSON *objB = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *shared = (cJSON *)0;
    cJSON *replacement = (cJSON *)0;
    cJSON *retrieved_childA = (cJSON *)0;
    cJSON *retrieved_items = (cJSON *)0;
    cJSON *retrieved_shared_ref = (cJSON *)0;
    cJSON_bool is_array_flag = 0;
    cJSON_bool added_objA = 0;
    cJSON_bool added_objB = 0;
    cJSON_bool added_items = 0;
    cJSON_bool added_shared_ref = 0;
    cJSON_bool replaced = 0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    objA = cJSON_CreateObject();
    objB = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    str1 = cJSON_CreateString("one");
    num2 = cJSON_CreateNumber(2.0);
    shared = cJSON_CreateString("shared_value");
    replacement = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added_objA = cJSON_AddItemToObject(objA, "num", num1);
    cJSON_AddItemToObject(objA, "label", str1);
    added_objB = cJSON_AddItemToObject(objB, "num2", num2);
    cJSON_AddItemToObject(objB, "shared_holder", cJSON_CreateObject());
    cJSON_AddItemToObject(objB->child, "inner_shared", shared); /* attach shared inside objB->child object */
    added_items = cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("elem0"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("elem1"));
    cJSON_AddItemToObject(root, "childA", objA);
    cJSON_AddItemToObject(root, "objB", objB);
    added_shared_ref = cJSON_AddItemReferenceToObject(root, "shared_ref", shared);

    // step 4: Operate
    retrieved_childA = cJSON_GetObjectItem(root, "childA");
    replaced = cJSON_ReplaceItemViaPointer(root, retrieved_childA, replacement);
    retrieved_items = cJSON_GetObjectItem(root, "items");
    is_array_flag = cJSON_IsArray(retrieved_items);
    arr_size = cJSON_GetArraySize(retrieved_items);
    retrieved_shared_ref = cJSON_GetObjectItem(root, "shared_ref");

    // step 5: Validate
    (void)added_objA;
    (void)added_objB;
    (void)added_items;
    (void)added_shared_ref;
    (void)replaced;
    (void)is_array_flag;
    (void)arr_size;
    (void)retrieved_shared_ref;
    (void)retrieved_childA;
    (void)retrieved_items;

    // step 6: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}