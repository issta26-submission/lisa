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
//<ID> 1000
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
    cJSON *opt_item = NULL;
    cJSON *active_item = NULL;
    cJSON *retrieved = NULL;
    char *retrieved_name = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("deviceX");
    cJSON_AddItemToObject(root, "name", name_item);
    opt_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", opt_item);
    active_item = cJSON_AddTrueToObject(root, "active");

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "name");
    retrieved_name = cJSON_GetStringValue(retrieved);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}