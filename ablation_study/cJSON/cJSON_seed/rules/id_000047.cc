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
//<ID> 47
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
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON *sum_item = NULL;
    cJSON *retrieved = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    int sz = 0;
    double val = 0.0;
    double sum = 0.0;
    cJSON_bool cmp = 0;

    // step 2: Setup - build object with array
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);

    // step 3: Operate / Validate - use array APIs and create derived item
    sz = cJSON_GetArraySize(arr);
    retrieved = cJSON_GetArrayItem(arr, 1);
    val = cJSON_GetNumberValue(retrieved);
    sum = (double)sz + val;
    sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "summary", sum_item);

    // step 4: Further validation, printing and cleanup
    dup_root = cJSON_Duplicate(root, 1);
    cmp = cJSON_Compare(root, dup_root, 1);
    printed = cJSON_PrintBuffered(root, 256, 0);
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}