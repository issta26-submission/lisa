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
//<ID> 434
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"names\":[\"Alice\",\"Bob\"],\"meta\":{\"count\":2}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, sizeof(json) - 1, &parse_end, 1);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *names = cJSON_GetObjectItemCaseSensitive(root, "names");
    cJSON *first_name_item = cJSON_GetArrayItem(names, 0);
    const char *first_name_str = cJSON_GetStringValue(first_name_item);

    // step 2: Configure
    cJSON *extras = cJSON_CreateArray();
    cJSON *eref1 = cJSON_CreateStringReference(first_name_str);
    cJSON *eref2 = cJSON_CreateStringReference("Charlie");
    cJSON_AddItemToArray(extras, eref1);
    cJSON_AddItemToArray(extras, eref2);
    cJSON_AddItemToObject(root, "extras", extras);

    // step 3: Operate
    cJSON *extras_found = cJSON_GetObjectItem(root, "extras");
    int extras_size = cJSON_GetArraySize(extras_found);
    cJSON *first_extra = cJSON_GetArrayItem(extras_found, 0);
    const char *first_extra_str = cJSON_GetStringValue(first_extra);
    cJSON_bool invalid_check = cJSON_IsInvalid(first_extra);

    // step 4: Validate & Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    (void)root_is_obj;
    (void)extras_size;
    (void)first_extra_str;
    (void)invalid_check;
    // API sequence test completed successfully
    return 66;
}