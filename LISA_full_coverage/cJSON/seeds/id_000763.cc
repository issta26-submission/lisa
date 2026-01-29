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
//<ID> 763
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char mutable_json[] = " { \"greeting\" : \"hello\", \"value\" : true } ";
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();

    // step 2: Configure
    cJSON *inner_bool = cJSON_AddBoolToObject(child, "inner_flag", 1);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObjectCS(child, "always_true", true_item);

    // step 3: Operate
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObjectCS(root, "child_ref", ref);
    cJSON *minified_str = cJSON_CreateString(mutable_json);
    cJSON_AddItemToObjectCS(root, "raw_minified", minified_str);
    cJSON_AddBoolToObject(root, "root_flag", 0);

    // step 4: Validate & Cleanup
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *printed = cJSON_PrintUnformatted(dup);
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    cJSON_Delete(child);
    // API sequence test completed successfully
    return 66;
}