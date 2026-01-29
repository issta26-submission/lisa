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
//<ID> 1094
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "initial", 1.5);
    const char *to_parse = "{\"name\":\"original\",\"value\":10}";
    size_t len = (sizeof("{\"name\":\"original\",\"value\":10}") - 1);
    cJSON *parsed = cJSON_ParseWithLength(to_parse, len);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    cJSON *parsed_obj = cJSON_GetObjectItemCaseSensitive(root, "parsed");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed_obj, "name");
    char *newstr = cJSON_SetValuestring(name_item, "modified");
    cJSON *added = cJSON_AddNumberToObject(root, "added_num", 99.0);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = newstr[0];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}