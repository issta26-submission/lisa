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
//<ID> 110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num_old = cJSON_CreateNumber(1.23);
    cJSON *num_new = cJSON_CreateNumber(4.56);
    cJSON *str = cJSON_CreateString("example");
    cJSON *nested_num = cJSON_CreateNumber(10.0);
    cJSON *retrieved_old = NULL;
    char *printed = NULL;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "old", num_old);
    cJSON_AddItemToObject(root, "text", str);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "value", nested_num);

    // step 3: Operate / Validate
    retrieved_old = cJSON_GetObjectItem(root, "old");
    replaced = cJSON_ReplaceItemViaPointer(root, retrieved_old, num_new);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}