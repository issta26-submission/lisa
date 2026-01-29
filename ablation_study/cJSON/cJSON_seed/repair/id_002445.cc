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
//<ID> 2445
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 10.0);
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "count2", count_val + 1.5);
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_flag = cJSON_IsTrue(active_item);
    buffer[1] = (char)(int)active_flag;
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[2] = printed2 && printed2[0] ? printed2[0] : '\0';

    // step 4: Validate and Cleanup
    cJSON *count2_item = cJSON_GetObjectItem(root, "count2");
    double count2_val = cJSON_GetNumberValue(count2_item);
    buffer[3] = (char)(int)count2_val;
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}