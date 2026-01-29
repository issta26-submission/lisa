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
//<ID> 1057
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "meta", "example");
    cJSON *s1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(items, s1);
    cJSON *s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(items, s2);

    // step 2: Configure
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_dup", items_dup);
    cJSON *extra = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extra", extra);
    cJSON_AddItemToArray(extra, cJSON_CreateString("more"));

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_AddStringToObject(root, "dump", printed);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}