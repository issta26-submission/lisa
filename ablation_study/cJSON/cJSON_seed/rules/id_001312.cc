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
//<ID> 1312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"existing\": 1}";
    const char *parse_end = NULL;
    cJSON_bool require_null_terminated = 1;
    cJSON *root = NULL;
    cJSON *new_arr = NULL;
    cJSON *got_arr = NULL;
    cJSON *raw_item = NULL;
    cJSON *dup_raw = NULL;
    cJSON *marker_bool = NULL;
    cJSON_bool is_raw = 0;
    cJSON_bool cmp_equal = 0;
    cJSON_bool arrs_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, require_null_terminated);
    new_arr = cJSON_AddArrayToObject(root, "new_arr");
    raw_item = cJSON_CreateRaw("unparsed:raw_payload");
    cJSON_AddItemToArray(new_arr, raw_item);
    got_arr = cJSON_GetObjectItem(root, "new_arr");

    // step 3: Operate / Validate
    is_raw = cJSON_IsRaw(raw_item);
    marker_bool = cJSON_CreateBool(is_raw);
    cJSON_AddItemToObject(root, "raw_was_raw", marker_bool);
    dup_raw = cJSON_Duplicate(raw_item, 1);
    cmp_equal = cJSON_Compare(raw_item, dup_raw, 1);
    arrs_equal = cJSON_Compare(new_arr, got_arr, 1);
    (void)cmp_equal;
    (void)arrs_equal;
    (void)parse_end;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_raw);
    // API sequence test completed successfully
    return 66;
}