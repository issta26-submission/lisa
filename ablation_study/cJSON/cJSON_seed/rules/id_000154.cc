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
//<ID> 154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_pi = cJSON_CreateNumber(3.1415);
    cJSON *num_repl = cJSON_CreateNumber(2.71828);
    cJSON *child = cJSON_CreateObject();
    const char raw_json[] = "{\"embedded\":true}";
    cJSON *raw_added = NULL;
    cJSON *got_pi = NULL;
    const char *raw_value = NULL;
    double value_after_replace = 0.0;
    cJSON_bool added_pi = 0;
    cJSON_bool added_child = 0;
    cJSON_bool replaced = 0;
    char *printed = NULL;

    // step 2: Setup / Configure
    added_pi = cJSON_AddItemToObject(root, "pi", num_pi);
    added_child = cJSON_AddItemToObject(root, "child", child);
    raw_added = cJSON_AddRawToObject(root, "raw", raw_json);

    // step 3: Operate / Validate
    got_pi = cJSON_GetObjectItem(root, "pi");
    replaced = cJSON_ReplaceItemViaPointer(root, got_pi, num_repl);
    got_pi = cJSON_GetObjectItem(root, "pi");
    value_after_replace = cJSON_GetNumberValue(got_pi);
    raw_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "raw"));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}