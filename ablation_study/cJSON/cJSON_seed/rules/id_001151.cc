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
//<ID> 1151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *nullitem = NULL;
    cJSON *parsed = NULL;
    cJSON *nested = NULL;
    cJSON *nested_ref = NULL;
    const char *parse_end = NULL;
    char json_buf[] = "{\"alpha\": 10, \"beta\": \"value\", \"nested\": {\"x\":1, \"y\":2}}";
    size_t json_len = (size_t)(sizeof(json_buf) - 1);
    char *dyn = NULL;
    double alpha_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "num", num);
    nullitem = cJSON_AddNullToObject(root, "none");
    dyn = (char *)cJSON_malloc(8);
    memset(dyn, 0, 8);
    dyn[0] = 'd';
    dyn[1] = 'y';
    dyn[2] = 'n';
    dyn[3] = '\0';
    str = cJSON_CreateString(dyn);
    cJSON_AddItemToObject(root, "dyn", str);

    // step 3: Operate / Validate
    cJSON_Minify(json_buf);
    parsed = cJSON_ParseWithLengthOpts(json_buf, json_len, &parse_end, 1);
    alpha_val = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "alpha"));
    cJSON_AddNumberToObject(root, "alpha_copy", alpha_val);
    nested = cJSON_GetObjectItem(parsed, "nested");
    nested_ref = cJSON_CreateObjectReference(nested);
    cJSON_AddItemToObject(root, "nested_ref", nested_ref);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(dyn);
    // API sequence test completed successfully
    return 66;
}