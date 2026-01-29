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
//<ID> 1346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"flag\": false, \"count\": 3}";
    cJSON *root = NULL;
    cJSON *count_item = NULL;
    cJSON *active_item = NULL;
    cJSON *true_item = NULL;
    char *printed1 = NULL;
    char *printed2 = NULL;
    double count_val = 0.0;
    cJSON_bool active_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "is_active", true_item);

    // step 3: Operate / Validate
    printed1 = cJSON_PrintBuffered(root, 256, 1);
    count_item = cJSON_GetObjectItem(root, "count");
    count_val = cJSON_GetNumberValue(count_item);
    active_item = cJSON_GetObjectItem(root, "is_active");
    active_flag = cJSON_IsTrue(active_item);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "is_active");
    printed2 = cJSON_PrintBuffered(root, 128, 1);
    (void)count_val;
    (void)active_flag;

    // step 4: Cleanup
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}