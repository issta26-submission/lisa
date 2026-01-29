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
//<ID> 1122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *elem = cJSON_CreateNumber(42.5);
    cJSON_AddItemToArray(arr, elem);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "scale", 2.5);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *note = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "note", note);

    // step 3: Operate and Validate
    cJSON *scale_item = cJSON_GetObjectItemCaseSensitive(root, "scale");
    double scale_val = cJSON_GetNumberValue(scale_item);
    cJSON *computed_item = cJSON_CreateNumber(scale_val * 10.0 + 0.5);
    cJSON_AddItemToObject(root, "computed", computed_item);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    scratch[0] = out[0];
    scratch[1] = out[1];
    cJSON_free(out);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}