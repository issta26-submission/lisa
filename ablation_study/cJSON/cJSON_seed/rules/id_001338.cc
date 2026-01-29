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
//<ID> 1338
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"metrics\": {\"count\": 42.0, \"name\": \"alpha\"}}";
    cJSON *root = NULL;
    cJSON *metrics = NULL;
    cJSON *count_item = NULL;
    cJSON *name_item = NULL;
    double count_value = 0.0;
    cJSON *new_number = NULL;
    cJSON *name_copy = NULL;
    char *serialized = NULL;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    metrics = cJSON_GetObjectItem(root, "metrics");
    count_item = cJSON_GetObjectItem(metrics, "count");
    name_item = cJSON_GetObjectItem(metrics, "name");

    // step 3: Operate / Validate
    count_value = cJSON_GetNumberValue(count_item);
    new_number = cJSON_CreateNumber(count_value + 1.0);
    name_copy = cJSON_CreateString(cJSON_GetStringValue(name_item));
    cJSON_AddItemToObject(root, "count_plus_one", new_number);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free(serialized);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}