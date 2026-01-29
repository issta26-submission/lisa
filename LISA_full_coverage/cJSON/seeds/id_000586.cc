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
//<ID> 586
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"person\": { \"name\": \"Alice\", \"age\": 30, \"active\": true }, \"tags\": [\"alpha\",\"beta\"] }";
    size_t raw_len = strlen(raw_json) + 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(raw_json, raw_len, &parse_end, 1);
    cJSON *person = cJSON_GetObjectItemCaseSensitive(root, "person");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(person, "name");
    char *name_value = cJSON_GetStringValue(name_item);

    // step 2: Configure
    char *old_name = cJSON_SetValuestring(name_item, "Alice Smith");
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_person = cJSON_GetObjectItemCaseSensitive(dup, "person");
    cJSON *dup_name_item = cJSON_GetObjectItemCaseSensitive(dup_person, "name");

    // step 3: Operate
    char *buffered = cJSON_PrintBuffered(root, 256, 1);
    int prealloc_len = 512;
    char *prealloc_buf = (char *)cJSON_malloc(prealloc_len);
    memset(prealloc_buf, 0, prealloc_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(dup, prealloc_buf, prealloc_len, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "tags");
    char *new_name_value = cJSON_GetStringValue(dup_name_item);
    double age_value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(person, "age"));

    // step 4: Validate & Cleanup
    (void)name_value;
    (void)old_name;
    (void)prealloc_ok;
    (void)new_name_value;
    (void)age_value;
    cJSON_free(buffered);
    cJSON_free(prealloc_buf);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}