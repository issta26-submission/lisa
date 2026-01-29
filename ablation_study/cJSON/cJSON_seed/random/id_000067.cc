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
//<ID> 67
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
    cJSON *str = cJSON_CreateString("example");
    cJSON *copied_ref = (cJSON *)0;
    char *retrieved = (char *)0;
    char *echo_retrieved = (char *)0;

    // step 2: Setup - populate array and attach to root
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - add a reference to the existing string item into root
    cJSON_AddItemReferenceToObject(root, "copied", str);

    // step 4: Operate/Validate - read the referenced string and echo it back into root
    copied_ref = cJSON_GetObjectItem(root, "copied");
    retrieved = cJSON_GetStringValue(copied_ref);
    cJSON_AddStringToObject(root, "echo", retrieved);
    echo_retrieved = cJSON_GetStringValue(cJSON_GetObjectItem(root, "echo"));

    // step 5: Cleanup - remove the echoed string explicitly, then delete the whole root
    cJSON_DeleteItemFromObject(root, "echo");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}