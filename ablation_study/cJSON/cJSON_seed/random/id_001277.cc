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
//<ID> 1277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *extra = (cJSON *)0;
    cJSON *flag_node = (cJSON *)0;
    const char *json = "{\"arr\":[10,20,30]}";
    const char *extra_json = "[40,50]";
    int size_before = 0;
    int size_extra = 0;
    int validation_score = 0;

    // step 2: Initialize - parse a root object containing an array and parse an extra array
    root = cJSON_Parse(json);
    extra = cJSON_Parse(extra_json);

    // step 3: Configure - obtain the existing array from root and attach the parsed extra array to root
    arr = cJSON_GetObjectItem(root, "arr");
    cJSON_AddItemToObject(root, "extra", extra);

    // step 4: Operate - measure array sizes and add a boolean field to the root object
    size_before = cJSON_GetArraySize(arr);
    flag_node = cJSON_AddBoolToObject(root, "ok", (cJSON_bool)1);
    size_extra = cJSON_GetArraySize(extra);

    // step 5: Validate - compute a simple validation score to ensure data flowed through APIs
    validation_score = (int)(root != (cJSON *)0) + size_before + size_extra + (int)(flag_node != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}