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
//<ID> 1008
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *null_item = NULL;
    cJSON *true_item = NULL;
    char *printed = NULL;
    const char *retrieved_name = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", null_item);
    true_item = cJSON_AddTrueToObject(root, "enabled");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    name_item = cJSON_GetObjectItem(root, "name");
    retrieved_name = cJSON_GetStringValue(name_item);
    (void)retrieved_name;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}