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
//<ID> 1002
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *meta = NULL;
    cJSON *name_item = NULL;
    cJSON *true_item = NULL;
    cJSON *null_item = NULL;
    cJSON *ver_item = NULL;
    cJSON *retrieved_name = NULL;
    const char *name_value = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("sensor-X");
    cJSON_AddItemToObject(root, "name", name_item);
    true_item = cJSON_AddTrueToObject(root, "active");
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", null_item);
    meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    ver_item = cJSON_CreateString("v1.2.3");
    cJSON_AddItemToObject(meta, "version", ver_item);

    // step 3: Operate / Validate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(retrieved_name);
    printed = cJSON_PrintUnformatted(root);
    (void)name_value; // keep value used to demonstrate data flow

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}