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
//<ID> 1340
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"flag\": true, \"value\": 42}";
    cJSON *root = NULL;
    cJSON *value_item = NULL;
    cJSON *true_item = NULL;
    char *printed = NULL;
    double value_num = 0.0;
    cJSON_bool flag_present = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    value_item = cJSON_GetObjectItem(root, "value");
    value_num = cJSON_GetNumberValue(value_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "flag");
    flag_present = cJSON_HasObjectItem(root, "flag");
    cJSON_AddNumberToObject(root, "flag_present", (double)flag_present);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}