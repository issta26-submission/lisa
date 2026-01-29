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
//<ID> 974
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize and Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObjectCS(config, "answer", num);
    cJSON *str = cJSON_CreateString("hello world");
    cJSON_AddItemToObjectCS(config, "greeting", str);

    // step 2: Operate (detach and move item)
    cJSON *detached = cJSON_DetachItemFromObject(config, "answer");
    cJSON *holder = cJSON_AddObjectToObject(root, "holder");
    cJSON_AddItemToObjectCS(holder, "moved_answer", detached);

    // step 3: Validate and Print
    cJSON *moved_item = cJSON_GetObjectItemCaseSensitive(holder, "moved_answer");
    double moved_value = cJSON_GetNumberValue(moved_item);
    cJSON *greet_item = cJSON_GetObjectItemCaseSensitive(config, "greeting");
    const char *greet_value = cJSON_GetStringValue(greet_item);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)moved_value;
    (void)greet_value;
    (void)printed;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}