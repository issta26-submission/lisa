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
//<ID> 417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[{\"id\":1},{\"id\":2}],\"meta\":{\"count\":2}}";
    size_t len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);
    cJSON *new_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(new_obj, "id", cJSON_CreateNumber(3));

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON *second_id = cJSON_GetObjectItemCaseSensitive(second, "id");
    double second_val = cJSON_GetNumberValue(second_id);
    cJSON_AddNumberToObject(root, "second_id_value", second_val);

    // step 3: Operate
    cJSON_InsertItemInArray(items, 2, new_obj);
    cJSON *inserted = cJSON_GetArrayItem(items, 2);
    cJSON *inserted_id = cJSON_GetObjectItemCaseSensitive(inserted, "id");
    double inserted_val = cJSON_GetNumberValue(inserted_id);
    cJSON_AddNumberToObject(root, "inserted_id_value", inserted_val);

    // step 4: Validate and Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}