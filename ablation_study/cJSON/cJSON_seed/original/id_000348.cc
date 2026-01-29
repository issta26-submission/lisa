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
//<ID> 348
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"Alice\",\"age\":30,\"scores\":[10,20,30]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *scores = cJSON_GetObjectItemCaseSensitive(parsed, "scores");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    cJSON *age = cJSON_GetObjectItemCaseSensitive(parsed, "age");

    // step 2: Configure (duplicate nodes to avoid shared ownership / double free)
    cJSON *root = cJSON_CreateObject();
    cJSON *collection = cJSON_CreateArray();
    cJSON *scores_dup = cJSON_Duplicate(scores, 1);
    cJSON *name_dup = cJSON_Duplicate(name, 1);
    cJSON_AddItemToArray(collection, scores_dup);
    cJSON_AddItemToArray(collection, name_dup);
    cJSON_AddItemToObject(root, "collection", collection);
    cJSON *age_dup = cJSON_Duplicate(age, 1);
    cJSON_AddItemToObject(root, "source_age", age_dup);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    return 66;
}