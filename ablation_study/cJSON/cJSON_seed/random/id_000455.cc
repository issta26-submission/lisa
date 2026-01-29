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
//<ID> 455
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *strItem = (cJSON *)0;
    cJSON *childRef = (cJSON *)0;
    char *printed = (char *)0;
    const char *got_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    strItem = cJSON_AddStringToObject(child, "label", "example");
    // Use a duplicate instead of an object reference to avoid double-free on cleanup
    childRef = cJSON_Duplicate(child, 1);
    cJSON_AddItemToArray(arr, childRef);
    cJSON_AddItemToObject(root, "node", child);
    cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    got_str = cJSON_GetStringValue((const cJSON *const)strItem);
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    (void)got_str;

    // step 5: Validate
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}