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
//<ID> 569
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"items\": [ {\"name\":\"one\",\"val\":1}, {\"name\":\"two\",\"val\":2} ], \"meta\": { \"count\": 2 } }";
    size_t raw_len = strlen(raw_json) + 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(raw_json, raw_len, &parse_end, 1);
    cJSON *items = cJSON_GetObjectItem(root, "items");

    // step 2: Configure
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *name_item = cJSON_GetObjectItem(first_item, "name");
    char *old_name = cJSON_SetValuestring(name_item, "ONE");
    cJSON *ref = cJSON_CreateObjectReference(first_item);
    cJSON_AddItemReferenceToArray(items, ref);

    // step 3: Operate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON_AddNumberToObject(root, "has_meta", (double)has_meta);
    char *buffered = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate & Cleanup
    const char *extracted_name = cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetObjectItem(root, "items"), 0));
    double extracted_count = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "meta"), "count"));
    (void)old_name;
    (void)extracted_name;
    (void)extracted_count;
    cJSON_free(buffered);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}