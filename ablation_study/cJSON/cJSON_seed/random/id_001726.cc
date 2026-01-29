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
//<ID> 1726
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *arr_item1 = (cJSON *)0;
    cJSON *arr_item2 = (cJSON *)0;
    char *retrieved_str = (char *)0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("hello");
    str2 = cJSON_CreateString("world");
    arr_item1 = cJSON_CreateString("one");
    arr_item2 = cJSON_CreateString("two");

    // step 3: Configure
    cJSON_AddItemToArray(arr, arr_item1);
    cJSON_AddItemToArray(arr, arr_item2);
    cJSON_AddItemToObject(root, "greeting", str1);
    cJSON_AddItemToObject(root, "target", str2);
    cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "list"));
    retrieved_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    validation = (int)(arr_size == 2) + (int)(retrieved_str != (char *)0);

    // step 5: Validate
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)str1;
    (void)str2;
    (void)arr_item1;
    (void)arr_item2;
    (void)retrieved_str;
    (void)arr_size;
    // API sequence test completed successfully
    return 66;
}