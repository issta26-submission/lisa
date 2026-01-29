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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("value_one");
    cJSON *num_item = cJSON_CreateNumber(1.0);
    cJSON *child = cJSON_CreateObject();
    cJSON *child_num = cJSON_CreateNumber(2.0);
    cJSON *retrieved = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    double before = 0.0;
    double after = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "n", num_item);
    cJSON_AddItemToObject(child, "inner", child_num);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "n");
    before = cJSON_GetNumberValue(retrieved);
    replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemViaPointer(root, retrieved, replacement);
    retrieved = cJSON_GetObjectItem(root, "n");
    after = cJSON_GetNumberValue(retrieved);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}