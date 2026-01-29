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
//<ID> 284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToArray(arr, num);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, flag);
    cJSON_AddItemToObject(root, "values", arr);

    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "values_ref", arr_dup);
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));

    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_bool = cJSON_IsBool(second);
    cJSON *ref_first = cJSON_GetArrayItem(arr_dup, 0);
    char *dump = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(64);
    memset(buffer, 0, 64);
    (void)first_val;
    (void)second_is_bool;
    (void)ref_first;

    cJSON_free(dump);
    cJSON_free(buffer);
    cJSON_Delete(root);

    return 66;
}