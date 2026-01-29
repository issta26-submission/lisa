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
//<ID> 1206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = nullptr;
    cJSON *arr = nullptr;
    cJSON *s1 = nullptr;
    cJSON *s2 = nullptr;
    cJSON *true_item = nullptr;
    cJSON *num_item = nullptr;
    cJSON *retrieved_num = nullptr;
    int arr_size = 0;
    double num_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array container
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - create string items, a true item and a numeric item, then assemble the object
    s1 = cJSON_CreateString("first");
    s2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);
    num_item = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("cJSON_test"));

    // step 4: Operate - retrieve array size and numeric value
    arr_size = cJSON_GetArraySize(arr);
    retrieved_num = cJSON_GetObjectItem(root, "value");
    num_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate - compute a small validation score
    validation_score = arr_size + (int)num_value;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}