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
//<ID> 1364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi_item);
    double updated_pi = cJSON_SetNumberHelper(pi_item, 6.28318);
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON *retrieved_pi = cJSON_GetObjectItem(root, "pi");
    double pi_value = cJSON_GetNumberValue(retrieved_pi);
    char *label_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    cJSON *double_pi_item = cJSON_CreateNumber(pi_value * 2.0 + updated_pi - pi_value);
    cJSON_AddItemToObject(root, "double_pi", double_pi_item);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON *printed_copy = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "printed", printed_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}