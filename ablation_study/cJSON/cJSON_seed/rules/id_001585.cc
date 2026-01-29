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
//<ID> 1585
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *config = NULL;
    cJSON *values = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *raw_item = NULL;
    cJSON *values_ref = NULL;
    cJSON *first = NULL;
    const char *first_str = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    config = cJSON_AddObjectToObject(root, "config");
    values = cJSON_AddArrayToObject(config, "values");
    item0 = cJSON_CreateString("one");
    item1 = cJSON_CreateString("two");
    cJSON_AddItemToArray(values, item0);
    cJSON_AddItemToArray(values, item1);
    raw_item = cJSON_AddRawToObject(root, "raw_json", "{\"inlined\":true,\"n\":5}");
    values_ref = cJSON_CreateObjectReference(values);
    cJSON_AddItemToObject(root, "values_ref", values_ref);

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(values, 0);
    first_str = cJSON_GetStringValue(first);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
}