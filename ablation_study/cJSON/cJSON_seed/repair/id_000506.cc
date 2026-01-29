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
//<ID> 506
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 5.0);
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    const cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_value = cJSON_GetNumberValue(count_item);
    char buf[64];
    int buf_len = snprintf(buf, sizeof(buf), "count=%.0f", count_value);
    (void)buf_len;
    cJSON *computed = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateString("replaced_label");
    cJSON *label_item = cJSON_GetObjectItem(root, "label");
    cJSON_ReplaceItemViaPointer(root, label_item, replacement);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *found = cJSON_GetObjectItem(root, "computed");
    const char *found_str = cJSON_GetStringValue(found);
    (void)found_str;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}