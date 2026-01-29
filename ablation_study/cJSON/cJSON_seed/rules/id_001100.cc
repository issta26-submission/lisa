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
//<ID> 1100
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *bool_item = NULL;
    cJSON *detached_count = NULL;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", num);
    str = cJSON_CreateString("processing");
    cJSON_AddItemToObject(root, "status", str);

    // step 3: Operate / Validate
    bool_item = cJSON_CreateBool(1);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "status", bool_item);
    detached_count = cJSON_DetachItemFromObject(root, "count");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached_count);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}