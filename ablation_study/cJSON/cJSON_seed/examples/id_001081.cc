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
//<ID> 1081
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON *ins = cJSON_CreateNumber(1.5);
    cJSON *rep = cJSON_CreateNumber(2.5);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_InsertItemInArray(arr, 1, ins);
    cJSON_ReplaceItemInArray(arr, 2, rep);

    // step 3: Operate & Validate
    cJSON *item = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_num = cJSON_IsNumber(item);
    double v = cJSON_GetNumberValue(item);
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    double sumv = v + v0 * (double)is_num;
    cJSON *summary = cJSON_CreateNumber(sumv);
    cJSON_AddItemToObject(root, "summary", summary);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}