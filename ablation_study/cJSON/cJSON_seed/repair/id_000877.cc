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
//<ID> 877
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
    cJSON *item0 = cJSON_CreateObject();
    cJSON *item1 = cJSON_CreateObject();
    cJSON_AddItemToArray(items, item0);
    cJSON_AddItemToArray(items, item1);

    // step 2: Configure
    cJSON *id0 = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(id0, 1.0);
    cJSON_AddItemToObject(item0, "id", id0);
    cJSON *val0 = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(val0, 10.5);
    cJSON_AddItemToObject(item0, "value", val0);
    cJSON *name0 = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(item0, "name", name0);

    cJSON *id1 = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(id1, 2.0);
    cJSON_AddItemToObject(item1, "id", id1);
    cJSON *val1 = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(val1, 20.25);
    cJSON_AddItemToObject(item1, "value", val1);
    cJSON *name1 = cJSON_CreateString("beta");
    cJSON_AddItemToObject(item1, "name", name1);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON *value_item = cJSON_GetObjectItem(second, "value");
    double extracted = cJSON_GetNumberValue(value_item);
    cJSON *copied = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(copied, extracted);
    cJSON_AddItemToObject(root, "copied_value", copied);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}