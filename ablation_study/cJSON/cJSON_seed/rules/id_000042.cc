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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON *s1 = cJSON_CreateString("example");
    cJSON *size_item = NULL;
    cJSON *second_elem = NULL;
    cJSON *dup_elem = NULL;
    char *printed = NULL;
    int arr_size = 0;
    cJSON_bool equal = 0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, s1);
    size_item = cJSON_AddNumberToObject(root, "array_size", (double)cJSON_GetArraySize(arr));

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    second_elem = cJSON_GetArrayItem(arr, 1);
    dup_elem = cJSON_Duplicate(second_elem, 1);
    cJSON_AddItemToObject(root, "duplicate_of_second", dup_elem);
    equal = cJSON_Compare(second_elem, dup_elem, 1);
    cJSON_AddBoolToObject(root, "second_equals_duplicate", equal);
    printed = cJSON_PrintBuffered(root, 128, 1);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}