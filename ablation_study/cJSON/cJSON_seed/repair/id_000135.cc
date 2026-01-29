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
//<ID> 135
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON_InsertItemInArray(items, 0, n0);
    cJSON *n1 = cJSON_CreateNumber(25.5);
    cJSON_InsertItemInArray(items, 1, n1);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(meta, "name", name);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "scale", 2.0);
    cJSON_SetValuestring(name, "updated_name");

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(got_items, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *scale_item = cJSON_GetObjectItem(root, "scale");
    double scale_val = cJSON_GetNumberValue(scale_item);
    double scaled = first_val * scale_val;
    cJSON_AddNumberToObject(root, "scaled_first", scaled);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}