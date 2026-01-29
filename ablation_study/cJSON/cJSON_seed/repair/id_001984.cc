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
//<ID> 1984
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(meta, "value", num);
    cJSON *label = cJSON_CreateString("sensorA");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    cJSON *meta_ref = cJSON_CreateObjectReference(meta);
    cJSON_AddItemToObject(root, "meta_ref", meta_ref);
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    cJSON *got_label = cJSON_GetObjectItem(meta, "label");
    const char *label_str = cJSON_GetStringValue(got_label);
    cJSON *got_value_item = cJSON_GetObjectItem(meta, "value");
    double got_value = cJSON_GetNumberValue(got_value_item);
    cJSON_bool is_flag_false = cJSON_IsFalse(flag);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (char)('A' + ((int)got_value % 26));
    buffer[1] = label_str[0];
    buffer[2] = (char)('0' + (is_flag_false % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}