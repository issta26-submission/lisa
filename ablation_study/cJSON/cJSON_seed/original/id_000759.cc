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
//<ID> 759
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_text = "{\"inner\":123}";
    const char *initial_name = "initial";
    cJSON *root = cJSON_CreateObject();
    cJSON *raw_item = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToObject(root, "rawdata", raw_item);
    cJSON *name_item = cJSON_CreateString(initial_name);
    cJSON_AddItemToObject(root, "name", name_item);

    // step 2: Configure
    cJSON *fetched_name = cJSON_GetObjectItem(root, "name");
    char *set_ret = cJSON_SetValuestring(fetched_name, "updated");

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "rawdata");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "name");
    char *printed = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}