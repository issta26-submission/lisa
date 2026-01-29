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
//<ID> 1916
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(data, "value", num_item);
    cJSON *label_item = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(data, "label", label_item);

    // step 2: Configure
    cJSON *replacement_num = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInObject(data, "value", replacement_num);
    char *set_ret = cJSON_SetValuestring(label_item, "updated_label");

    // step 3: Operate and Validate
    cJSON *got_value = cJSON_GetObjectItem(data, "value");
    double numeric = cJSON_GetNumberValue(got_value);
    cJSON *got_label = cJSON_GetObjectItem(data, "label");
    char *label_str = cJSON_GetStringValue(got_label);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + ((int)numeric % 10));
    buffer[2] = label_str[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    (void)set_ret;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}