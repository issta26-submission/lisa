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
//<ID> 2215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "value", num);

    // step 2: Configure
    double newval = cJSON_SetNumberHelper(num, 3.1415);
    cJSON *copy = cJSON_CreateNumber(newval);
    cJSON_AddItemToObject(root, "pi_copy", copy);
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate and Validate
    cJSON_bool is_bool = cJSON_IsBool(flag);
    (void)is_bool;
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}