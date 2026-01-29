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
//<ID> 1198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "title", "example");
    cJSON_AddNullToObject(root, "optional");
    cJSON *count = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON_SetNumberHelper(count, 123.0);
    cJSON *temp = cJSON_CreateString("transient");
    cJSON_AddItemToObject(root, "transient", temp);

    // step 2: Configure
    const char json_text[] = "{\"alpha\":1,\"beta\":\"x\"}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, sizeof(json_text) - 1, &parse_end, (cJSON_bool)1);
    cJSON *parsed_dup = cJSON_Duplicate(parsed, (cJSON_bool)1);
    cJSON *detached = cJSON_DetachItemViaPointer(root, temp);
    cJSON_AddItemToObject(parsed_dup, "moved", detached);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    double alpha_val = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed_dup, "alpha"));
    double total = count_val + alpha_val;
    cJSON_AddNumberToObject(root, "sum", total);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(parsed_dup);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}