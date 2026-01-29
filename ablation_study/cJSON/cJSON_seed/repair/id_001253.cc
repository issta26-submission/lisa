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
//<ID> 1253
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[10,20,30],\"title\":\"initial\"}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *newstr = cJSON_CreateString("temp");
    cJSON_SetValuestring(newstr, "updated");

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *item1 = cJSON_GetArrayItem(items, 1);
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "dup_items", dup_items);
    cJSON_AddItemToObject(root, "new_string", newstr);

    // step 3: Operate and Validate
    cJSON *dup_item0 = cJSON_GetArrayItem(dup_items, 0);
    double v1 = cJSON_GetNumberValue(item1);
    double vdup0 = cJSON_GetNumberValue(dup_item0);
    double sum = v1 + vdup0;
    cJSON *sum_num = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_num);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}