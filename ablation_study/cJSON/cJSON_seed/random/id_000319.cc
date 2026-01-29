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
//<ID> 319
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
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *got_child = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    char *got_str = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();

    // step 3: Configure
    str_item = cJSON_CreateString("hello_world");
    cJSON_AddItemToObject(child, "greeting", str_item);
    cJSON_AddItemToObject(root, "metadata", child);
    true_item = cJSON_AddTrueToObject(root, "success");

    // step 4: Operate
    got_child = cJSON_GetObjectItem(root, "metadata");
    got_str = cJSON_GetStringValue(cJSON_GetObjectItem(got_child, "greeting"));
    dup = cJSON_Duplicate(root, (cJSON_bool)1);

    // step 5: Validate
    cJSON_AddItemToObject(dup, "copied_note", cJSON_CreateString(got_str));
    cJSON_GetObjectItem(dup, "success");

    // step 6: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}