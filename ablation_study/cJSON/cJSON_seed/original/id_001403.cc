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
//<ID> 1403
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int ints[] = {1, 2, 3, 4};
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *int_array = cJSON_CreateIntArray(ints, 4);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "number", num_item);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObject(root, "label", str_item);

    // step 2: Configure
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON *got_number = cJSON_GetObjectItem(root, "number");
    cJSON *got_label = cJSON_GetObjectItem(root, "label");
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    double number_value = cJSON_GetNumberValue(got_number);
    char *label_value = cJSON_GetStringValue(got_label);
    cJSON_bool enabled_value = cJSON_IsTrue(got_enabled);
    (void)number_value;
    (void)label_value;
    (void)enabled_value;
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}