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
//<ID> 616
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
    cJSON *sref = (cJSON *)0;
    cJSON *num_added = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *alias_item = (cJSON *)0;
    cJSON_bool is_false = 0;
    double retrieved_num = 0.0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    sref = cJSON_CreateStringReference("hello");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 4: Operate
    num_added = cJSON_AddNumberToObject(root, "count", 3.1415);
    cJSON_AddItemReferenceToObject(root, "alias", sref);
    alias_item = cJSON_GetObjectItem(root, "alias");
    is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));

    // step 5: Validate
    retrieved_num = cJSON_GetNumberValue(num_added);
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "items"));
    (void)alias_item;
    (void)is_false;
    (void)retrieved_num;
    (void)arr_size;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}