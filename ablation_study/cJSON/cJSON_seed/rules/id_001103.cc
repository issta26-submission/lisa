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
//<ID> 1103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *orig = NULL;
    cJSON *backup = NULL;
    cJSON *num = NULL;
    cJSON *newbool = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    const char *orig_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    orig = cJSON_CreateString("initial_value");
    cJSON_AddItemToObject(root, "flag", orig);
    num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "to_remove", num);

    // step 3: Operate / Validate
    orig_str = cJSON_GetStringValue(orig);
    backup = cJSON_CreateString(orig_str);
    newbool = cJSON_CreateBool(1);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", newbool);
    detached = cJSON_DetachItemFromObject(root, "to_remove");
    cJSON_AddItemToObject(root, "restored", detached);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(backup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}