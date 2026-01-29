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
//<ID> 1348
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"status\": \"ok\", \"count\": 2}";
    cJSON *root = NULL;
    cJSON *active_item = NULL;
    cJSON *true_item = NULL;
    char *printed_before = NULL;
    char *printed_after = NULL;
    cJSON_bool was_active = 0;
    const int buffer_size = 256;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_item);

    // step 3: Operate / Validate
    printed_before = cJSON_PrintBuffered(root, buffer_size, 1);
    active_item = cJSON_GetObjectItemCaseSensitive(root, "active");
    was_active = cJSON_IsTrue(active_item);
    cJSON_AddBoolToObject(root, "was_active", was_active);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "active");
    printed_after = cJSON_PrintBuffered(root, buffer_size, 1);

    // step 4: Cleanup
    cJSON_free(printed_before);
    cJSON_free(printed_after);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}