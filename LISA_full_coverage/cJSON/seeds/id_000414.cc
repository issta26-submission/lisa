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
//<ID> 414
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[1,2,3],\"meta\":{\"name\":\"test\"}}";
    const size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *new_number = cJSON_CreateNumber(second_val + 10.0);
    cJSON *new_obj = cJSON_CreateObject();
    cJSON_AddStringToObject(new_obj, "tag", "inserted");
    cJSON_AddItemToObject(root, "newobj", new_obj);

    // step 3: Operate
    cJSON_InsertItemInArray(items, 1, new_number);

    // step 4: Validate and Cleanup
    cJSON *inserted = cJSON_GetArrayItem(items, 1);
    double inserted_val = cJSON_GetNumberValue(inserted);
    (void)inserted_val;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}