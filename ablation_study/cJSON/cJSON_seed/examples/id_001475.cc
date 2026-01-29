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
//<ID> 1475
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root1 = cJSON_CreateObject();
    cJSON *child1 = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child1, "n", num);
    cJSON_AddItemToObject(child1, "s", str);
    cJSON_AddItemToObject(root1, "child", child1);

    cJSON *root2 = cJSON_CreateObject();
    cJSON *child2 = cJSON_Duplicate(child1, 1);
    cJSON_AddItemToObject(root2, "child", child2);

    // step 2: Configure
    cJSON *ver = cJSON_CreateString(cJSON_Version());
    cJSON_AddItemToObject(root2, "version", ver);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root1, "child");
    cJSON_bool is_obj = cJSON_IsObject(detached);
    cJSON_bool equal = cJSON_Compare(detached, child2, 1);
    char *printed = cJSON_PrintUnformatted(root2);
    (void)is_obj;
    (void)equal;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_DeleteItemFromObjectCaseSensitive(root2, "child");
    cJSON_Delete(detached);
    cJSON_Delete(root1);
    cJSON_Delete(root2);

    // API sequence test completed successfully
    return 66;
}