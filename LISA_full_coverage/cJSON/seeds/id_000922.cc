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
//<ID> 922
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"example\",\"active\":false}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *added_flag = cJSON_AddBoolToObject(root, "configured", 1);
    cJSON *dup = cJSON_Duplicate(root, 1);

    // step 3: Operate
    cJSON *flag_in_dup = cJSON_GetObjectItem(dup, "configured");
    cJSON_bool is_false = cJSON_IsFalse(flag_in_dup);
    char *printed = cJSON_PrintBuffered(dup, 128, 1);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}