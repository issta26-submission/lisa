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
//<ID> 596
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON *label = cJSON_CreateString("sample");
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON_AddBoolToObject(root, "visible", 0);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "created_by", cJSON_CreateString("tester"));
    cJSON_AddItemToObject(meta, "priority", cJSON_CreateNumber(5.0));

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON *a0 = cJSON_GetArrayItem(items, 0);
    cJSON *a1 = cJSON_GetArrayItem(items, 1);
    cJSON *a2 = cJSON_GetArrayItem(items, 2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2);
    double average = sum / (double)count;
    cJSON_AddItemToObject(root, "sum", cJSON_CreateNumber(sum));
    cJSON_AddItemToObject(root, "average", cJSON_CreateNumber(average));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}