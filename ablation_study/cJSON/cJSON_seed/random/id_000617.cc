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
//<ID> 617
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
    cJSON *sref1 = (cJSON *)0;
    cJSON *sref2 = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *num_item1 = (cJSON *)0;
    cJSON *num_item2 = (cJSON *)0;
    cJSON_bool is_false = 0;
    int arr_size = 0;
    const char *label_val = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    sref1 = cJSON_CreateStringReference("hello");
    sref2 = cJSON_CreateStringReference("world");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, sref1);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "flag", false_item);
    num_item1 = cJSON_AddNumberToObject(root, "pi", 3.14);
    cJSON_AddItemReferenceToObject(root, "label", sref2);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    num_item2 = cJSON_AddNumberToObject(root, "count", (double)arr_size);
    is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    label_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));

    // step 5: Validate
    (void)label_val;
    (void)is_false;
    (void)num_item1;
    (void)num_item2;
    (void)arr_size;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}