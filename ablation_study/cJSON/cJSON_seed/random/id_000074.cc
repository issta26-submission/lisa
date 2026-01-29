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
//<ID> 74
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
    cJSON *flag = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_item = (cJSON *)0;
    const char *first_val = (const char *)0;
    int arr_size = 0;
    const char *const strings[] = { "one", "two" };

    // step 2: Initialize / Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateStringArray(strings, 2);
    flag = cJSON_CreateTrue();

    // step 3: Configure - attach items and add a reference to the array
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemReferenceToObject(root, "array_ref", arr);
    cJSON_AddNumberToObject(root, "initial_marker", 3.14);

    // step 4: Operate - read from the array and derive values
    retrieved_arr = cJSON_GetObjectItem(root, "array");
    first_item = cJSON_GetArrayItem(retrieved_arr, 0);
    first_val = cJSON_GetStringValue(first_item);
    arr_size = cJSON_GetArraySize(retrieved_arr);

    // step 5: Validate - store derived values back into the root
    cJSON_AddNumberToObject(root, "array_count", (double)arr_size);
    cJSON_AddItemToObject(root, "first_value", cJSON_CreateString(first_val));

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}