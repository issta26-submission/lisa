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
//<ID> 1105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *orig_flag = NULL;
    cJSON *temp_item = NULL;
    cJSON *new_bool = NULL;
    cJSON *detached = NULL;
    char *printed_root = NULL;
    char *printed_detached = NULL;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    orig_flag = cJSON_CreateString("initial_value");
    cJSON_AddItemToObject(root, "flag", orig_flag);
    temp_item = cJSON_CreateString("will_be_detached");
    cJSON_AddItemToObject(root, "temp", temp_item);

    // step 3: Operate / Validate
    new_bool = cJSON_CreateBool(1);
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", new_bool);
    detached = cJSON_DetachItemFromObject(root, "temp");
    printed_root = cJSON_PrintUnformatted(root);
    printed_detached = cJSON_PrintUnformatted(detached);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}