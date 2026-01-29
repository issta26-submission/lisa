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
//<ID> 1167
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *config = (cJSON *)0;
    cJSON *flags = (cJSON *)0;
    cJSON *btrue = (cJSON *)0;
    cJSON *bfalse = (cJSON *)0;
    cJSON *status = (cJSON *)0;
    cJSON *fetched_config = (cJSON *)0;
    cJSON *fetched_flags = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *validation_marker = (cJSON *)0;
    int score = 0;

    // step 2: Initialize - create root and nested config object and attach
    root = cJSON_CreateObject();
    config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);

    // step 3: Configure - create flags array with boolean elements and add status string
    flags = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "flags", flags);
    btrue = cJSON_CreateTrue();
    bfalse = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, btrue);
    cJSON_AddItemToArray(flags, bfalse);
    status = cJSON_CreateString("all-good");
    cJSON_AddItemToObject(root, "status", status);

    // step 4: Operate - fetch objects and array items, check types
    fetched_config = cJSON_GetObjectItem(root, "config");
    fetched_flags = cJSON_GetObjectItem(fetched_config, "flags");
    elem0 = cJSON_GetArrayItem(fetched_flags, 0);
    elem1 = cJSON_GetArrayItem(fetched_flags, 1);
    score = (int)cJSON_IsBool(elem0) + (int)cJSON_IsBool(elem1);

    // step 5: Validate - record validation score into the JSON structure
    validation_marker = cJSON_CreateNumber((double)score);
    cJSON_AddItemToObject(root, "validation_score", validation_marker);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)score;
    return 66;
}