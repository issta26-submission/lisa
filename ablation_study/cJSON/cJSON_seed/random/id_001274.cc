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
//<ID> 1274
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
    cJSON *added_bool = (cJSON *)0;
    cJSON *extra = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;
    const char *json = "{\"items\":[10,20,30]}";

    // step 2: Initialize - parse a JSON string into a root object
    root = cJSON_Parse(json);

    // step 3: Configure - obtain the array child from the parsed root
    arr = cJSON_GetObjectItem(root, "items");

    // step 4: Operate - measure array size, add a boolean and an extra string item to the root, re-measure
    size_before = cJSON_GetArraySize(arr);
    added_bool = cJSON_AddBoolToObject(root, "enabled", 1);
    extra = cJSON_CreateString("note");
    cJSON_AddItemToObject(root, "note", extra);
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a simple validation score to ensure values flowed through APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + size_before + size_after + (int)(added_bool != (cJSON *)0) + (int)(extra != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}