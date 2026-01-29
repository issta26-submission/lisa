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
//<ID> 49
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
    cJSON *n1 = cJSON_CreateNumber(3.0);
    cJSON *n2 = cJSON_CreateNumber(7.0);
    cJSON *dup_root = NULL;
    cJSON *first_elem = NULL;
    cJSON *calc_item = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double first_val = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    dup_root = cJSON_Duplicate(root, 1);

    // step 3: Operate / Validate
    equal = cJSON_Compare(root, dup_root, 1);
    arr_size = cJSON_GetArraySize(arr);
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_elem);
    calc_item = cJSON_CreateNumber(first_val + (double)arr_size + (double)equal);
    cJSON_AddItemToObject(root, "calculation", calc_item);
    printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}