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
//<ID> 1721
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
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *retrieved_label = (cJSON *)0;
    char *label_value = (char *)0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem1 = cJSON_CreateString("one");
    elem2 = cJSON_CreateString("two");
    label = cJSON_CreateString("root_label");

    // step 3: Configure
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "label", label);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "items"));
    retrieved_label = cJSON_GetObjectItem(root, "label");
    label_value = cJSON_GetStringValue(retrieved_label);
    validation = arr_size + (label_value != (char *)0);

    // step 5: Validate
    (void)validation;
    (void)label_value;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)elem1;
    (void)elem2;
    (void)label;
    (void)retrieved_label;
    // API sequence test completed successfully
    return 66;
}