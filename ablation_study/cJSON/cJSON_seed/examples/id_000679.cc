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
//<ID> 679
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *array_primary = cJSON_CreateArray();
    cJSON *array_refs = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *num_dup = cJSON_Duplicate(num_item, 0);

    // step 2: Configure
    cJSON_AddItemToArray(array_primary, num_item);
    cJSON_AddItemReferenceToArray(array_refs, num_dup);
    cJSON_AddItemToObject(root, "primary", array_primary);
    cJSON_AddItemToObject(root, "refs", array_refs);
    cJSON_AddNumberToObject(root, "count", 1.0);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double new_count = cJSON_SetNumberHelper(count_item, 7.0);
    (void)version;
    (void)new_count;

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_primary = cJSON_GetObjectItemCaseSensitive(parsed, "primary");
    int parsed_size = cJSON_GetArraySize(parsed_primary);
    cJSON *first_item = cJSON_GetArrayItem(parsed_primary, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    (void)parsed_size;
    (void)first_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}