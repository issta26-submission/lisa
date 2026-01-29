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
//<ID> 1091
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"initial\",\"count\":1}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);

    // step 2: Configure
    cJSON *added = cJSON_AddNumberToObject(root, "added", 3.14);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON_SetValuestring(name_item, "modified");

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *mem = cJSON_malloc((size_t)32);
    memset(mem, 0, 32);
    char *scratch = (char *)mem;
    scratch[0] = out[0];
    scratch[1] = out[1];
    scratch[2] = (char)('0' + (((int)added->valuedouble) & 0xF));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(mem);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}