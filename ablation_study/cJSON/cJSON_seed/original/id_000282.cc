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
//<ID> 282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(-2.25);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);

    cJSON *arr_ref = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "numbers_ref", arr_ref);

    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_bool = cJSON_IsBool(first);
    double first_val = cJSON_GetNumberValue(first);
    char *out = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(64);
    int summary = (int)first_is_bool + (int)first_val;
    (void)summary;

    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);

    return 66;
}