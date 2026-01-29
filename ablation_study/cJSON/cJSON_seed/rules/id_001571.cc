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
//<ID> 1571
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *dup = NULL;
    cJSON *pi_item = NULL;
    cJSON *nested = NULL;
    cJSON *v_item = NULL;
    cJSON *copystr = NULL;
    const char *json = "{\"pi\":3.1415, \"nested\": {\"v\": 42}}";
    double pi_val = 0.0;
    double v_val = 0.0;
    cJSON_bool are_equal = 0;
    char *out = NULL;
    char *copy = NULL;
    size_t len = 0;

    // step 2: Setup / Parse
    root = cJSON_Parse(json);
    dup = cJSON_Parse(json);
    pi_item = cJSON_GetObjectItem(root, "pi");
    nested = cJSON_GetObjectItem(root, "nested");
    v_item = cJSON_GetObjectItem(nested, "v");

    // step 3: Operate / Validate
    pi_val = cJSON_GetNumberValue(pi_item);
    v_val = cJSON_GetNumberValue(v_item);
    are_equal = cJSON_Compare(root, dup, 1);
    out = cJSON_PrintUnformatted(root);
    len = (out != NULL) ? (strlen(out) + 1) : 1;
    copy = (char *)cJSON_malloc(len);
    memset(copy, 0, len);
    if (out) memcpy(copy, out, len);
    copystr = cJSON_CreateString(copy);
    cJSON_AddItemToObject(root, "dump", copystr);

    // step 4: Cleanup
    cJSON_free(copy);
    cJSON_free(out);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}