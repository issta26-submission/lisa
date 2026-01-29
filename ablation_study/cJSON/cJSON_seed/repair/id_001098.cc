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
//<ID> 1098
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char json[] = "{\"name\":\"orig\",\"n\":1}";
    cJSON *parsed = cJSON_ParseWithLength(json, sizeof(json) - 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    char *updated = cJSON_SetValuestring(name_item, "updated");
    cJSON_AddNumberToObject(root, "added", 3.1415);

    // step 3: Operate and Validate
    char *unformatted = cJSON_PrintUnformatted(parsed);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = unformatted[0];
    cJSON *added_item = cJSON_GetObjectItemCaseSensitive(root, "added");
    double added_value = cJSON_GetNumberValue(added_item);
    ((char *)scratch)[1] = (char)((int)added_value);
    ((char *)scratch)[2] = updated[0];

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(scratch);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}