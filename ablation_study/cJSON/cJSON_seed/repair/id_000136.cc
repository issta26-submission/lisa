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
//<ID> 136
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
    cJSON_AddItemToArray(items, n0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(items, n2);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "count", 2.0);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name);
    char *set_res = cJSON_SetValuestring(name, "updated");
    (void)set_res;

    // step 3: Operate and Validate
    cJSON *insert_num = cJSON_CreateNumber(20.0);
    cJSON_InsertItemInArray(items, 1, insert_num);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *count_item = cJSON_GetObjectItem(meta, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    double scaled = first_val * count_val;
    cJSON_AddNumberToObject(root, "scaled_first", scaled);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}