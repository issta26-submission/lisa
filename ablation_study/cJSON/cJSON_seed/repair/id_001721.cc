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
//<ID> 1721
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
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(config, "num", num_item);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag_true);
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON_AddNullToObject(config, "optional");
    double updated_val = cJSON_SetNumberHelper(num_item, 100.5);
    (void)updated_val;

    // step 3: Operate and Validate
    cJSON *retrieved_num = cJSON_GetObjectItem(config, "num");
    double num_value = cJSON_GetNumberValue(retrieved_num);
    cJSON_bool is_true_flag = cJSON_IsTrue(flag_true);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = is_true_flag ? 'T' : 'F';
    buffer[2] = (char)('0' + ((int)num_value % 10));
    (void)buffer;
    (void)num_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}