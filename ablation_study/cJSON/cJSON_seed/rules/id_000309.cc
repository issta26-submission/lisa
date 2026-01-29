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
//<ID> 309
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON *bool_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON *str_item = cJSON_CreateString("hello");
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    char *printed = NULL;
    cJSON_bool is_bool = 0;
    const char *str_val = NULL;
    double num_value = 0.0;
    cJSON *retrieved_cfg = NULL;
    cJSON *retrieved_bool = NULL;
    cJSON *retrieved_num = NULL;
    cJSON *retrieved_str = NULL;
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON_AddItemToObject(cfg, "enabled", bool_item);
    cJSON_AddItemToObject(cfg, "threshold", num_item);
    cJSON_AddItemToObject(cfg, "greeting", str_item);

    // step 3: Operate / Validate
    retrieved_cfg = cJSON_GetObjectItem(root, "config");
    retrieved_bool = cJSON_GetObjectItem(retrieved_cfg, "enabled");
    is_bool = cJSON_IsBool(retrieved_bool);
    retrieved_num = cJSON_GetObjectItem(retrieved_cfg, "threshold");
    num_value = cJSON_GetNumberValue(retrieved_num);
    retrieved_str = cJSON_GetObjectItem(retrieved_cfg, "greeting");
    str_val = cJSON_GetStringValue(retrieved_str);
    printed = cJSON_PrintUnformatted(root);
    printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 0);
    buffer[0] = (char)('0' + (int)(is_bool));
    buffer[1] = (char)('A' + ((int)num_value % 26));
    buffer[2] = str_val[0];

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}