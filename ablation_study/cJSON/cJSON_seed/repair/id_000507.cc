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
//<ID> 507
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *label_old = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(info, "label", label_old);
    cJSON_AddNumberToObject(info, "count", 4.0);

    // step 2: Configure
    cJSON *label_new = cJSON_CreateString("replaced_label");
    (void)cJSON_ReplaceItemViaPointer(info, label_old, label_new);

    // step 3: Operate
    const cJSON *count_item = cJSON_GetObjectItem(info, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    double scaled_val = count_val * 2.5;
    cJSON_AddNumberToObject(root, "scaled", scaled_val);
    char buf[64];
    int blen = snprintf(buf, sizeof(buf), "scaled=%.2f", scaled_val);
    (void)blen;
    cJSON *computed = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "computed", computed);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    const cJSON *scaled_item = cJSON_GetObjectItem(root, "scaled");
    double check = cJSON_GetNumberValue(scaled_item);
    (void)check;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}