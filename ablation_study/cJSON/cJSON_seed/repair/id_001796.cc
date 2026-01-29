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
//<ID> 1796
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, true_item);
    cJSON *true_dup = cJSON_Duplicate(true_item, 0);
    cJSON_AddItemToArray(flags, true_dup);
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *threshold = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(config, "threshold", threshold);

    // step 2: Configure
    cJSON *replacement_true = cJSON_CreateTrue();
    cJSON_ReplaceItemInObject(config, "threshold", replacement_true);
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_backup", config_copy);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    int flags_count = cJSON_GetArraySize(flags);
    cJSON *first_flag = cJSON_GetArrayItem(flags, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first_flag);
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *th_item = cJSON_GetObjectItem(cfg, "threshold");
    cJSON_bool th_is_true = cJSON_IsTrue(th_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (flags_count % 10));
    buffer[2] = first_is_true ? 'T' : 'F';
    buffer[3] = th_is_true ? 'Y' : 'N';
    (void)flags_count;
    (void)first_flag;
    (void)first_is_true;
    (void)th_item;
    (void)th_is_true;
    (void)cfg;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}