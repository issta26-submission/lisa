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
//<ID> 1476
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(child, "flag_true", true_item);
    cJSON_AddItemToObject(child, "flag_false", false_item);
    cJSON *name_item = cJSON_CreateString("embedded");
    cJSON_AddItemToObject(child, "name", name_item);
    cJSON *count_item = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(child, "count", count_item);
    cJSON_SetNumberHelper(count_item, 42.0);
    cJSON_AddNumberToObject(root, "root_val", 3.14);

    // step 3: Operate and Validate
    cJSON *got_name = cJSON_GetObjectItem(child, "name");
    char *name_str = cJSON_GetStringValue(got_name);
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *root_val_item = cJSON_GetObjectItem(root, "root_val");
    cJSON_SetNumberHelper(root_val_item, 7.77);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = name_str ? name_str[0] : 'n';
    scratch[1] = (char)('0' + ((int)count_val % 10));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('A');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}