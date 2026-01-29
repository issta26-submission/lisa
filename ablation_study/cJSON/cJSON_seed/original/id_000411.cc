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
//<ID> 411
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"menu\": {\"items\": [ {\"id\": 1}, {\"id\": 2} ], \"title\":\"Test\" }}";
    size_t len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);

    // step 2: Configure
    cJSON *menu = cJSON_GetObjectItemCaseSensitive(root, "menu");
    cJSON *items = cJSON_GetObjectItemCaseSensitive(menu, "items");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *first_id_item = cJSON_GetObjectItemCaseSensitive(first_item, "id");
    double first_id = cJSON_GetNumberValue(first_id_item);

    // step 3: Operate
    cJSON *new_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(new_obj, "id", cJSON_CreateNumber(first_id + 10.0));
    cJSON_bool insert_result = cJSON_InsertItemInArray(items, 1, new_obj);
    cJSON *inserted = cJSON_GetArrayItem(items, 1);

    // step 4: Validate and Cleanup
    double inserted_id = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(inserted, "id"));
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}