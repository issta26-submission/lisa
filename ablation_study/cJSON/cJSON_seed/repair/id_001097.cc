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
//<ID> 1097
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    char input[] = "{\"external\":{\"value\":7},\"count\":5}";
    cJSON *parsed = cJSON_ParseWithLength(input, sizeof(input) - 1);

    // step 2: Configure
    cJSON_AddStringToObject(root, "label", "original");
    cJSON *label = cJSON_GetObjectItemCaseSensitive(root, "label");
    cJSON_SetValuestring(label, "updated");
    cJSON_AddNumberToObject(root, "local_count", 3.14);

    // step 3: Operate and Validate
    cJSON *ext = cJSON_GetObjectItemCaseSensitive(parsed, "external");
    cJSON *ext_val = cJSON_GetObjectItemCaseSensitive(ext, "value");
    double extracted = cJSON_GetNumberValue(ext_val);
    cJSON_AddNumberToObject(root, "external_value", extracted);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}