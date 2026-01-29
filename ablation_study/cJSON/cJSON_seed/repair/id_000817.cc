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
//<ID> 817
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *metrics = cJSON_AddObjectToObject(config, "metrics");
    cJSON_AddNumberToObject(metrics, "value", 123.456);
    cJSON *items = cJSON_AddArrayToObject(config, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(3.0));

    // step 2: Configure
    const char *json_text = "{\"external\": {\"val\": 9.5, \"meta\": \"ok\"}}";
    cJSON *parsed_temp = cJSON_Parse(json_text);
    cJSON *detached_external = cJSON_DetachItemFromObject(parsed_temp, "external");
    cJSON_AddItemToObject(root, "external", detached_external);

    // step 3: Operate and Validate
    cJSON *value_item = cJSON_GetObjectItemCaseSensitive(metrics, "value");
    double value_num = cJSON_GetNumberValue(value_item);
    int items_count = cJSON_GetArraySize(items);
    double computed = value_num + (double)items_count;
    cJSON_AddNumberToObject(root, "computed", computed);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_temp = cJSON_PrintUnformatted(parsed_temp);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_temp);
    cJSON_Delete(parsed_temp);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}