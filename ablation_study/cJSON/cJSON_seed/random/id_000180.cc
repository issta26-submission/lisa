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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *orig = (cJSON *)0;
    cJSON *repl = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    const char *retrieved_str = (const char *)0;
    cJSON_bool replaced = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    orig = cJSON_CreateString("initial_value");
    repl = cJSON_CreateString("replacement_value");

    // step 3: Configure
    cJSON_AddItemToObject(root, "greet", orig);
    cJSON_AddFalseToObject(root, "replaced");

    // step 4: Operate
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "greet", repl);
    retrieved = cJSON_GetObjectItem(root, "greet");
    retrieved_str = cJSON_GetStringValue(retrieved);

    // step 5: Validate
    cJSON *replacement_marker = cJSON_CreateString(retrieved_str);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "replaced", replacement_marker);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}