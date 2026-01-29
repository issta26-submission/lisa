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
//<ID> 302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *parsed = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *dup0 = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    char *out1 = (char *)0;
    char *out2 = (char *)0;
    const char *parse_end = (const char *)0;
    char json[] = "[ false , true ]";
    cJSON_bool require_null = (cJSON_bool)1;

    // step 2: Initialize
    parsed = cJSON_ParseWithOpts(json, &parse_end, require_null);
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    item0 = cJSON_GetArrayItem(parsed, 0);
    cJSON_AddItemToObject(root, "arr", parsed);
    cJSON_AddItemToObject(root, "temp", false_item);
    dup0 = cJSON_Duplicate(item0, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "first", dup0);

    // step 4: Operate
    out1 = cJSON_PrintUnformatted(root);

    // step 5: Validate
    cJSON_DeleteItemFromObject(root, "temp");
    out2 = cJSON_PrintUnformatted(root);

    // step 6: Cleanup
    cJSON_free(out1);
    cJSON_free(out2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}