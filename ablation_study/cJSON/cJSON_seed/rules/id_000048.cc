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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(20.25);
    cJSON *count_item = NULL;
    cJSON *first_elem = NULL;
    cJSON *copy_of_first = NULL;
    cJSON *computed = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double first_val = 0.0;
    cJSON_bool cmp = 0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_elem);
    copy_of_first = cJSON_CreateNumber(first_val);
    cmp = cJSON_Compare(first_elem, copy_of_first, 1);
    cJSON_Delete(copy_of_first);
    count_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count_item);
    computed = cJSON_CreateNumber(first_val + cJSON_GetNumberValue(count_item));
    cJSON_AddItemToObject(root, "sum", computed);
    printed = cJSON_PrintBuffered(root, 128, 1);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}