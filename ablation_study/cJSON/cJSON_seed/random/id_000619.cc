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
//<ID> 619
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
    cJSON *false_item = (cJSON *)0;
    cJSON *added_count = (cJSON *)0;
    cJSON *retrieved_count = (cJSON *)0;
    cJSON_bool is_false = 0;
    double count_val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    sref = cJSON_CreateStringReference("hello");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, sref);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemReferenceToObject(root, "label", sref);
    added_count = cJSON_AddNumberToObject(root, "count", 42.0);

    // step 4: Operate
    is_false = cJSON_IsFalse(false_item);
    retrieved_count = cJSON_GetObjectItem(root, "count");
    count_val = cJSON_GetNumberValue(retrieved_count);

    // step 5: Validate
    (void)is_false;
    (void)count_val;
    (void)added_count;
    (void)retrieved_count;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}