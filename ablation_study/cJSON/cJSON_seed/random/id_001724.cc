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
//<ID> 1724
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *arr_item1 = (cJSON *)0;
    cJSON *arr_item2 = (cJSON *)0;
    cJSON *retrieved_greeting = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    char *greeting_value = (char *)0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello");
    arr = cJSON_CreateArray();
    arr_item1 = cJSON_CreateString("one");
    arr_item2 = cJSON_CreateString("two");

    // step 3: Configure
    cJSON_AddItemToArray(arr, arr_item1);
    cJSON_AddItemToArray(arr, arr_item2);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    greeting_value = (char *)cJSON_GetStringValue(retrieved_greeting);
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    arr_size = cJSON_GetArraySize(retrieved_arr);

    // step 5: Validate
    validation = (int)(greeting_value != (char *)0) + (int)(arr_size == 2);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)greeting;
    (void)arr;
    (void)arr_item1;
    (void)arr_item2;
    (void)retrieved_greeting;
    (void)retrieved_arr;
    (void)greeting_value;
    (void)arr_size;
    // API sequence test completed successfully
    return 66;
}