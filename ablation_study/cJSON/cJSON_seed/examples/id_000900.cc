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
//<ID> 900
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", nullable);
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *active_bool = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_Minify(snapshot);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON *equal_flag = cJSON_CreateNumber((double)equal);
    cJSON_AddItemToObject(root, "equal_flag", equal_flag);
    cJSON *parsed_dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "parsed_dup", parsed_dup);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}