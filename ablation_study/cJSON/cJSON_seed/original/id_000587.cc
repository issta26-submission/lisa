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
//<ID> 587
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"name\":\"alpha\",\"values\":[1,2,3],\"meta\":{\"count\":3}}";
    size_t raw_len = strlen(raw_json) + 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(raw_json, raw_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *old_name = cJSON_SetValuestring(name_item, "ALPHA");
    cJSON *dup = cJSON_Duplicate(root, 1);

    // step 3: Operate
    char *buffered = cJSON_PrintBuffered(root, 256, 1);
    int pre_len = 512;
    char *preallocated = (char *)cJSON_malloc(pre_len);
    memset(preallocated, 0, pre_len);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(dup, preallocated, pre_len, 1);
    char *dup_name = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(dup, "name"));
    cJSON_AddStringToObject(root, "dup_name", dup_name);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "values");

    // step 4: Validate & Cleanup
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    (void)equal;
    (void)old_name;
    (void)pre_ok;
    cJSON_free(buffered);
    cJSON_free(preallocated);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}