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
//<ID> 1917
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *param = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(config, "param", param);
    cJSON *label = cJSON_CreateString("old");
    cJSON_AddItemToObject(config, "label", label);

    // step 2: Configure
    cJSON *new_param = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInObject(config, "param", new_param);
    char *prev_label = cJSON_SetValuestring(label, "updated");
    (void)prev_label;

    // step 3: Operate and Validate
    cJSON *got_param = cJSON_GetObjectItem(config, "param");
    double param_val = cJSON_GetNumberValue(got_param);
    cJSON *got_label = cJSON_GetObjectItem(config, "label");
    char *label_str = cJSON_GetStringValue(got_label);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + ((int)param_val % 10));
    buffer[2] = label_str ? label_str[0] : 'X';
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}