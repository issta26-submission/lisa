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
//<ID> 415
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    const char json[] = "{\"items\":[{\"id\":1},{\"id\":2}],\"meta\":{\"count\":2}}";
    const char *parse_end = NULL;
    size_t json_len = (sizeof(json) - 1);
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 0);
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON *new_item = cJSON_CreateObject();
    cJSON *new_id = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(new_item, "id", new_id);
    cJSON *meta_count = cJSON_GetObjectItemCaseSensitive(meta, "count");
    cJSON_InsertItemInArray(items, 1, new_item);
    cJSON *inserted = cJSON_GetArrayItem(items, 1);
    cJSON *inserted_id = cJSON_GetObjectItemCaseSensitive(inserted, "id");
    double inserted_val = cJSON_GetNumberValue(inserted_id);
    double updated_count = cJSON_GetNumberValue(meta_count) + 1.0;
    cJSON_SetNumberHelper(meta_count, updated_count);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66;
}