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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize (parse initial JSON)
    const char *json = "{\"name\":\"tester\",\"value\":42}";
    const char *parse_end1 = NULL;
    cJSON *parsed1 = cJSON_ParseWithOpts(json, &parse_end1, 1);

    // step 2: Configure (duplicate parsed object and augment with booleans)
    cJSON *dup = cJSON_Duplicate(parsed1, 1);
    cJSON_AddTrueToObject(dup, "is_active");
    cJSON_AddFalseToObject(dup, "is_deleted");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", dup);

    // step 3: Operate and Validate (serialize and re-parse, duplicate parsed result)
    char *serialized = cJSON_PrintUnformatted(root);
    const char *parse_end2 = NULL;
    cJSON *reparsed = cJSON_ParseWithOpts(serialized, &parse_end2, 1);
    cJSON *reparsed_dup = cJSON_Duplicate(reparsed, 0);
    cJSON_Delete(reparsed_dup);

    // step 4: Cleanup
    cJSON_free(serialized);
    cJSON_Delete(reparsed);
    cJSON_Delete(parsed1);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}