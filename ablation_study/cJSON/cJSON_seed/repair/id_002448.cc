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
//<ID> 2448
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
    cJSON *true_flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_flag);
    cJSON_AddNumberToObject(root, "count", 42.0);
    cJSON_AddNumberToObject(child, "value", 3.14);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *val_item = cJSON_GetObjectItem(got_child, "value");
    double val = cJSON_GetNumberValue(val_item);
    buffer[1] = (char)(int)val;
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    buffer[2] = (char)(int)count_val;
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    buffer[3] = flag_item && flag_item->type ? (char)1 : (char)0;

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[4] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}