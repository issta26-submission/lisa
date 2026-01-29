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
//<ID> 1107
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
    cJSON *flag = NULL;
    cJSON *new_flag = NULL;
    cJSON *detached = NULL;
    char *root_str = NULL;
    char *detached_str = NULL;
    double detached_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(3.14159);
    flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "pi", num);
    cJSON_AddItemToObject(root, "active", flag);

    // step 3: Operate / Validate
    new_flag = cJSON_CreateBool(0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "active", new_flag);
    detached = cJSON_DetachItemFromObject(root, "pi");
    detached_value = cJSON_GetNumberValue(detached);
    root_str = cJSON_PrintUnformatted(root);
    detached_str = cJSON_PrintUnformatted(detached);

    // step 4: Cleanup
    cJSON_free(root_str);
    cJSON_free(detached_str);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}