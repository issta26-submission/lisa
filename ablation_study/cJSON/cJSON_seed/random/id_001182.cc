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
//<ID> 1182
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
    cJSON *author_item = (cJSON *)0;
    cJSON *version_item = (cJSON *)0;
    char *serialized = (char *)0;
    char *author_str = (char *)0;
    double version_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root and nested meta object, add author and version
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "author", "Alice");
    cJSON_AddItemToObject(root, "meta", meta);
    version_item = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "version", version_item);

    // step 3: Operate - retrieve nested items case-sensitively and extract values
    meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    author_item = cJSON_GetObjectItemCaseSensitive(meta, "author");
    author_str = cJSON_GetStringValue(author_item);
    version_item = cJSON_GetObjectItemCaseSensitive(root, "version");
    version_val = cJSON_GetNumberValue(version_item);

    // step 4: Serialize - produce an unformatted JSON string and free it
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void *)serialized);

    // step 5: Validate - compute a simple score from extracted values
    validation_score = (int)version_val + (int)author_str[0];

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}