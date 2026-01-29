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
//<ID> 935
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *retrieved = NULL;
    char *printed = NULL;
    cJSON_bool invalid_flag = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", child);
    cJSON_AddStringToObject(root, "salutation", "hi");

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "greeting");
    invalid_flag = cJSON_IsInvalid(retrieved);
    child->valueint = (int)invalid_flag;
    root->valueint = child->valueint;
    printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObject(root, "salutation");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}