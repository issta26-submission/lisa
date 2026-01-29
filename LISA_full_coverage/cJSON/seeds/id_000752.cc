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
//<ID> 752
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_name = "original";
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString(initial_name);
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(nested, "field", cJSON_CreateString("value"));
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON *raw = cJSON_CreateRaw("{\"raw_key\":123}");
    cJSON_AddItemToObject(root, "rawdata", raw);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromObject(root, "nested");
    char *set_ret1 = cJSON_SetValuestring(name_item, "updated");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "rawdata");
    cJSON_AddItemToObject(root, "restored", detached);

    // step 3: Operate
    cJSON *raw_from_name = cJSON_CreateRaw(set_ret1);
    cJSON_AddItemToObject(root, "name_raw", raw_from_name);
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    cJSON *raw_from_print = cJSON_CreateRaw(printed);
    cJSON_AddItemToObject(root, "as_raw", raw_from_print);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}