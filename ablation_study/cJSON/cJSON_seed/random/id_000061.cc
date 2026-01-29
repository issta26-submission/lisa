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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *plain = cJSON_CreateString("plain_string");
    cJSON *shared_ref = cJSON_CreateStringReference("shared_value");
    cJSON *retrieved_ref = (cJSON *)0;
    const char *val_plain = (const char *)0;
    const char *val_shared = (const char *)0;

    // step 2: Setup - attach array and plain string to root
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "plain", plain);

    // step 3: Configure - add shared_ref by reference
    cJSON_AddItemReferenceToObject(root, "shared", shared_ref);

    // step 4: Operate/Validate - retrieve string values
    retrieved_ref = cJSON_GetObjectItem(root, "shared");
    val_shared = cJSON_GetStringValue(retrieved_ref);
    val_plain = cJSON_GetStringValue(cJSON_GetObjectItem(root, "plain"));

    // step 5: Cleanup - remove plain and delete root
    cJSON_DeleteItemFromObject(root, "plain");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}