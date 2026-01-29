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
//<ID> 1311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"name\":\"example\",\"values\":[1,2,3]}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *orig_values = NULL;
    cJSON *added_array = NULL;
    cJSON *raw_item = NULL;
    cJSON *num_item = NULL;
    cJSON *dup_root = NULL;
    cJSON_bool raw_flag = 0;
    cJSON_bool equal_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    orig_values = cJSON_GetObjectItem(root, "values");
    added_array = cJSON_AddArrayToObject(root, "added_array");
    raw_item = cJSON_CreateRaw("/*raw*/42");
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(added_array, raw_item);
    cJSON_AddItemToArray(added_array, num_item);

    // step 3: Operate / Validate
    raw_flag = cJSON_IsRaw(raw_item);
    dup_root = cJSON_Duplicate(root, 1);
    equal_flag = cJSON_Compare(root, dup_root, 1);
    (void)orig_values;
    (void)parse_end;
    (void)raw_flag;
    (void)equal_flag;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}