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
//<ID> 985
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *nums = NULL;
    cJSON *vals = NULL;
    cJSON *first = NULL;
    char *mutable_json = NULL;
    char *printed = NULL;
    char *prebuf = NULL;
    const char raw_json[] = "{\"device\":\"sensorA\",\"vals\":[10.5,20.25],\"flag\":false}";
    size_t raw_len = (size_t)(sizeof(raw_json) - 1);
    int prelen = 128;
    double first_val = 0.0;

    // step 2: Setup / Configure
    mutable_json = (char *)cJSON_malloc(raw_len + 1);
    memset(mutable_json, 0, raw_len + 1);
    memcpy(mutable_json, raw_json, raw_len);
    cJSON_Minify(mutable_json);
    parsed = cJSON_Parse(mutable_json);
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "source", "unit42");
    nums = cJSON_AddArrayToObject(root, "numbers");
    cJSON_AddItemToArray(nums, cJSON_CreateNumber(7.0));
    cJSON_AddItemToObject(root, "ok", cJSON_CreateFalse());
    cJSON_AddItemToObject(root, "snapshot", parsed);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    prebuf = (char *)cJSON_malloc((size_t)prelen);
    memset(prebuf, 0, prelen);
    cJSON_PrintPreallocated(root, prebuf, prelen, 0);
    vals = cJSON_GetObjectItem(parsed, "vals");
    first = cJSON_GetArrayItem(vals, 0);
    first_val = cJSON_GetNumberValue(first);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(prebuf);
    cJSON_free(mutable_json);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}