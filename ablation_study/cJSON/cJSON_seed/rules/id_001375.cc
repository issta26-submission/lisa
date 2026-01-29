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
//<ID> 1375
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *first_elem = NULL;
    cJSON *dup_elem = NULL;
    cJSON *true_flag = NULL;
    char *printed = NULL;
    int array_size = 0;
    cJSON_bool compare_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "total", 3.0);
    true_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_flag);

    // step 3: Operate / Validate
    array_size = cJSON_GetArraySize(arr);
    first_elem = cJSON_GetArrayItem(arr, 0);
    dup_elem = cJSON_Duplicate(first_elem, 1);
    compare_ok = cJSON_Compare(first_elem, dup_elem, 1);
    printed = cJSON_PrintUnformatted(root);
    (void)array_size;
    (void)compare_ok;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_elem);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}