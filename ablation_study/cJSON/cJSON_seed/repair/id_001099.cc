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
//<ID> 1099
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"orig\",\"count\":1}";
    cJSON *root = cJSON_ParseWithLength(json, (size_t)(sizeof("{\"name\":\"orig\",\"count\":1}") - 1));

    // step 2: Configure
    cJSON *added = cJSON_AddNumberToObject(root, "added", 3.14);
    cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *newname = cJSON_SetValuestring(nameItem, "updated");

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = newname[0];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}