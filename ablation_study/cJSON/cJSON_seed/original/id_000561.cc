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
//<ID> 561
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"items\": [ { \"id\": 1, \"name\": \"one\" }, { \"id\": 2, \"name\": \"two\" } ], \"meta\": { \"count\": 2 } }";
    size_t raw_len = strlen(raw_json) + 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(raw_json, raw_len, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON *second_name = cJSON_GetObjectItem(second_item, "name");
    cJSON_SetValuestring(second_name, "second");
    cJSON_bool add_ref_result = cJSON_AddItemReferenceToArray(items, first_item);
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *meta_flag = cJSON_CreateNumber((double)has_meta);
    cJSON_AddItemToObject(root, "has_meta", meta_flag);
    cJSON *ref_flag = cJSON_CreateNumber((double)add_ref_result);
    cJSON_AddItemToObject(root, "ref_added", ref_flag);

    // step 3: Operate
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);
    size_t snapshot_len = snapshot ? strlen(snapshot) : 0;
    (void)snapshot_len;

    // step 4: Validate & Cleanup
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "meta"), "count"));
    (void)count_val;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}