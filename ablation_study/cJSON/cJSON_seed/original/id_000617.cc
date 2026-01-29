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
//<ID> 617
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *a = cJSON_CreateNumber(10.0);
    cJSON *b = cJSON_CreateNumber(20.0);
    cJSON_AddItemToObject(metrics, "a", a);
    cJSON_AddItemToObject(metrics, "b", b);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *c = cJSON_CreateNumber(5.0);
    cJSON_AddItemToArray(list, c);

    // step 2: Configure
    double a_val = cJSON_GetNumberValue(a);
    double b_val = cJSON_GetNumberValue(b);
    double sum_val = a_val + b_val;
    cJSON *sum_item = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON *detached_a = cJSON_DetachItemFromObjectCaseSensitive(metrics, "a");

    // step 3: Operate
    cJSON_AddItemReferenceToArray(list, detached_a);
    char *unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "sum");
    cJSON_DeleteItemFromObject(metrics, "b");
    cJSON_free(unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}