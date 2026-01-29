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
//<ID> 40
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *retrieved0 = NULL;
    cJSON *retrieved2 = NULL;
    cJSON *dup_item = NULL;
    cJSON *validation_num = NULL;
    char *buffered = NULL;
    int arr_size = 0;
    cJSON_bool cmp_result = 0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "mixed", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, s2);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    retrieved2 = cJSON_GetArrayItem(arr, 2);
    dup_item = cJSON_Duplicate(retrieved2, 1);
    cmp_result = cJSON_Compare(retrieved2, dup_item, 1);
    validation_num = cJSON_CreateNumber((double)arr_size + (double)cmp_result);
    cJSON_AddItemToObject(root, "validation", validation_num);
    buffered = cJSON_PrintBuffered((const cJSON *)root, 256, 1);

    // step 4: Cleanup
    cJSON_free(buffered);
    cJSON_Delete(dup_item);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}