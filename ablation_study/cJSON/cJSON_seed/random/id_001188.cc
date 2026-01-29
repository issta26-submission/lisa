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
//<ID> 1188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *id_item = (cJSON *)0;
    char *name_str = (char *)0;
    char *id_str = (char *)0;
    char *serialized = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and nested meta object with string members
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "fuzztester");
    meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "id", "alpha");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate - retrieve items using case-sensitive lookup and extract string values
    name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    name_str = cJSON_GetStringValue(name_item);
    meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    id_item = cJSON_GetObjectItemCaseSensitive(meta, "id");
    id_str = cJSON_GetStringValue(id_item);

    // step 4: Validate - compute a simple validation score and serialize the structure
    validation_score = (int)name_str[0] + (int)id_str[0];
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void *)serialized);

    // step 5: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}