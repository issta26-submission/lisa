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
//<ID> 472
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *raw_payload = cJSON_CreateRaw("{\"x\":10}");
    cJSON_AddItemToObject(root, "raw_payload", raw_payload);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddNumberToObject(root, "count", 42.0);

    // step 2: Configure
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_AddNumberToObject(dup, "dup_marker", 1.0);

    // step 3: Operate
    char *printed_root = cJSON_Print(root);
    char *printed_dup = cJSON_PrintUnformatted(dup);
    cJSON_free(printed_root);
    cJSON_free(printed_dup);

    // step 4: Validate and Cleanup
    (void)flag_is_false;
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}