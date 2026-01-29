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
//<ID> 928
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
    cJSON *name_item = cJSON_CreateString("device-old");
    cJSON *num_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, num_item);

    // step 2: Configure
    char *snapshot_before = cJSON_PrintUnformatted(root);

    // step 3: Operate & Validate
    cJSON *got_num = cJSON_GetArrayItem(arr, 0);
    cJSON_SetNumberHelper(got_num, 42.5);
    cJSON *new_name = cJSON_CreateString("device-new");
    cJSON_ReplaceItemInObject(root, "name", new_name);
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    char *snapshot_after = cJSON_PrintUnformatted(root);
    (void)got_name; // value used to maintain meaningful flow; snapshot_after reflects the change

    // step 4: Cleanup
    cJSON_free(snapshot_before);
    cJSON_free(snapshot_after);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}